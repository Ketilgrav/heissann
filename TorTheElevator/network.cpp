#include "network.h"

void NetworkMessage::UDP_init_socket_receive(){
	//Initialize receive socket address
	sockaddr_in receiveAddress;
	receiveAddress.sin_family = AF_INET;
	receiveAddress.sin_port = htons(receivePort);
	receiveAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	socklen_t receiveAddressLength = sizeof(receiveAddress);

	//Initialize and bind listen socket
	if ( (receiveSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Failed to create receiveSocket\n");
		sleep(2);
		exit(1);
	}

	fcntl(receiveSocket, F_SETFL, O_NONBLOCK); //Set non-blocking

	if (bind(receiveSocket, (struct sockaddr *)&receiveAddress, receiveAddressLength) < 0){
		perror("Failed to bind receiveSocket");
		sleep(2);
		exit(1);
	}
}

void NetworkMessage::UDP_init_socket_send(){
	sendAddress.sin_family = AF_INET;
	sendAddress.sin_port = htons(sendPort);
	inet_aton(broadcastIp, &sendAddress.sin_addr);

	//Initialize and set up sendSocket
	if ( (sendSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Socket not created\n");
		sleep(2);
		exit(1);
	}
	int broadcastPermission = 1;
	if (setsockopt(sendSocket, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0){
		perror("sendSocket broadcast enable failed");
		sleep(2);
		exit(1);
	}
	int reusePermission = 1;
	if (setsockopt(sendSocket, SOL_SOCKET, SO_REUSEADDR, (void *)&reusePermission, sizeof(reusePermission)) < 0){
		perror("sendSocket reuse address enable failed");
		sleep(2);
		exit(1);
	}
}



bool NetworkMessage::UDP_send(){
	ssize_t sentBytes = sendto(sendSocket, &sendMsg, sizeof(sendMsg), 0, (struct sockaddr *)&sendAddress, sizeof(sendAddress));
	if (sentBytes < 0){
		perror("Sending failed");
		return 0;
	}
	if (sentBytes < sizeof(sendMsg)){
		perror("Incomplete send");
		return 0;
	}
	return 1;
}


void NetworkMessage::send_message(const void* sendMsg){
	this->sendMsg = sendMsg;
	this->sendMsg->set_send_time();
	UDP_send();
}

bool NetworkMessage::UDP_receive(){
	//Legg til, beskjeden forkastes om checksum er false, eller tiden har gått ut

	ssize_t recvBytes = recvfrom(receiveSocket, &receiveMsg, sizeof(receiveMsg), 0, nullptr, NULL);
	if(recvBytes == 0){
		perror("Receive connection closed remotely");
		sleep(2);
		exit(1);
		return 0;
	}
		//Error from socket
	else if (recvBytes == -1) {
		//No data on socket
		if (errno == EWOULDBLOCK || errno == EAGAIN) {
			return 0;
		}
		else {
			perror("Receive failed");
			sleep(2);
			exit(1);
			return 0;
		}

	}
	return 1;
}

bool NetworkMessage::receive_message(void* receiveMsg){
	this->receiveMsg = receiveMsg;
	if(!UDP_receive()){
		return 0;
	}
	if(this->receiveMsg->read_time() + messageTimeoutTime < time(NULL)){
		return 0;
	}
	return 1;
}


NetworkMessage::NetworkMessage(int receivePort, int sendPort, const char broadcastIp[], time_t messageTimeoutTime){
	this->messageTimeoutTime = messageTimeoutTime;
	this->receivePort = receivePort;
	this->sendPort = sendPort;
	strcpy(this->broadcastIp, broadcastIp);
	UDP_init_socket_receive();
	UDP_init_socket_send();

	// Magical code to get local IP adress
	int fd;
	struct ifreq ifr;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
	ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);
	//
	unsigned long localIP = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr;
	this->networkID = localIP>>8*3;

	printf("Network ID: %i\n", this->networkID);


}
