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



void NetworkMessage::send_message(const void* sendMsg, size_t msg_size){
	uint8_t data[sizeof(time_t)+msg_size];
	time_t now  =time(NULL);
	memcpy(data,&now, sizeof(time_t));
	memcpy(data+sizeof(time_t),sendMsg,msg_size);
	ssize_t sentBytes = sendto(sendSocket, &data, sizeof(data), 0, (struct sockaddr *)&sendAddress, sizeof(sendAddress));
	if (sentBytes < 0){
		std::cout << "Sending failed." << std::endl;
	}
	else if (sentBytes < sizeof(sendMsg)){
		std::cout << "Incomplete send." << std::endl;
	}
}


bool NetworkMessage::receive_message(void* receiveMsg, size_t msg_size){
	uint8_t buffer[sizeof(time_t)+msg_size];
	ssize_t recvBytes = recvfrom(receiveSocket, &buffer, sizeof(buffer), 0, nullptr, NULL);
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
	else{
	}



	time_t sendTime;
	memcpy(&sendTime,buffer,sizeof(time_t));

	if(sendTime + messageTimeoutTime < time(NULL)){
		std::cout << "Message timed out due to sendtime(" << sendTime << ") + messageTimeoutTime(" << messageTimeoutTime << ") < time now(" << time(NULL) << ")" <<std::endl;
		return 0;
	}

	memcpy(receiveMsg,buffer+sizeof(time_t),msg_size);

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
