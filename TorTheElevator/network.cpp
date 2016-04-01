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
		abort(1);
	}

	fcntl(receiveSocket, F_SETFL, O_NONBLOCK); //Set non-blocking

	if (bind(receiveSocket, (struct sockaddr *)&receiveAddress, receiveAddressLength) < 0){
		perror("Failed to bind receiveSocket");
		sleep(2);
		abort(1);
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
		abort(1);
	}
	int broadcastPermission = 1;
	if (setsockopt(sendSocket, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0){
		perror("sendSocket broadcast enable failed");
		sleep(2);
		abort(1);
	}
	int reusePermission = 1;
	if (setsockopt(sendSocket, SOL_SOCKET, SO_REUSEADDR, (void *)&reusePermission, sizeof(reusePermission)) < 0){
		perror("sendSocket reuse address enable failed");
		sleep(2);
		abort(1);
	}
}


bool NetworkMessage::UDP_checksum(){
	return ((receiveMsg.msgType+receiveMsg.floor+receiveMsg.button+receiveMsg.price+receiveMsg.sendTime+receiveMsg.checkSum)%255 == 0);
}

void NetworkMessage::UDP_make_checksum(){
	sendMsg.checkSum = 255 - (sendMsg.msgType+sendMsg.floor+sendMsg.button+sendMsg.price+sendMsg.sendTime)%255;
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

void NetworkMessage::send_message(MessageType msgType, uint8_t floor, uint8_t button, uint16_t price, time_t sendTime){
	sendMsg.floor = floor;
	sendMsg.msgType = msgType;
	sendMsg.button = button;
	sendMsg.price = price;
	sendMsg.sendTime = sendTime;
	UDP_make_checksum();
	UDP_send();
}

void NetworkMessage::send_message(const Message* msgOut){
	sendMsg = *msgOut;
	UDP_make_checksum();
	UDP_send();
}

bool NetworkMessage::UDP_receive(){
	//Legg til, beskjeden forkastes om checksum er false, eller tiden har gått ut
	ssize_t recvBytes = recvfrom(receiveSocket, &receiveMsg, sizeof(receiveMsg), 0, nullptr, NULL);
	if(recvBytes == 0){
		perror("Receive connection closed remotely");
		sleep(2);
		abort(1);
		return 0;
	}
		//Error from socket
	else if (recvBytes == -1){
		//No data on socket
		if (errno == EWOULDBLOCK || errno == EAGAIN){
			return 0;
		}
		else{
			perror("Receive failed");
			sleep(2);
			abort(1);
			return 0;
		}
	}
	return 1;
}

bool NetworkMessage::receive_message(){
	if(!UDP_receive()){
		return 0;
	}
	if(!UDP_checksum()){
		return 0;
	}
	if(receiveMsg.sendTime + TIMEOUT_TIME < time(NULL)){
		return 0;
	}
	return 1;
}


NetworkMessage::NetworkMessage(int receivePort, int sendPort, const char broadcastIp[]){
	this->receivePort = receivePort;
	this->sendPort = sendPort;
	strcpy(this->broadcastIp, broadcastIp);
	UDP_init_socket_receive();
	UDP_init_socket_send();
}
