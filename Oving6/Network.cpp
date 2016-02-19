#include "Network.h"


int UDP_init_socket_recieve(int port){
	//Initialize recieve socket address
	struct sockaddr_in recieveAddress;
	recieveAddress.sin_family = AF_INET;
	recieveAddress.sin_port = htons(port);
	recieveAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	socklen_t receiveAddressLength = sizeof(recieveAddress);

	//Initialize and bind listen socket
	int recieveSocket;
	if ( (recieveSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Failed to create recieveSocket\n");
		return -1;
	}
	timeval receiveTimeOut;
	receiveTimeOut.tv_sec = TIMEOUT_SEC;
	receiveTimeOut.tv_usec = TIMEOUT_USEC;
	if(	setsockopt(recieveSocket, SOL_SOCKET, SO_RCVTIMEO, &receiveTimeOut, sizeof(receiveTimeOut)) < 0 ){
		perror("Failed to set receive timeout");
		return -1;
	}
	if (bind(recieveSocket, (struct sockaddr *)&recieveAddress, receiveAddressLength) < 0){
		perror("Failed to bind recieveSocket");
		return -1;
	}
	return recieveSocket;
}

int UDP_init_socket_send(sockaddr_in* sendAddress, int port){
	sendAddress->sin_family = AF_INET;
	sendAddress->sin_port = htons(port);
	inet_aton(broadcastIP, &sendAddress->sin_addr);

	//Initialize and set up sendSocket
	int sendSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if ( sendSocket < 0){
		perror("Socket not created\n");
		return -1;
	}
	int broadcastPermission = 1;
	if (setsockopt(sendSocket, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0){
		perror("sendSocket broadcast enable failed");
		return -1;
	}
	int reusePermission = 1;
	if (setsockopt(sendSocket, SOL_SOCKET, SO_REUSEADDR, (void *)&reusePermission, sizeof(reusePermission)) < 0){
		perror("sendSocket reuse address enable failed");
		return -1;
	}

	return sendSocket;
}

bool UDP_checksum(){ //DUMMY!
	return true;
}

int UDP_send(int socket, void* message, sockaddr_in* sendAddress){
	if ( sendto(socket, message, 1024, 0, (struct sockaddr *)sendAddress, sizeof(*sendAddress)) < 0){
		perror("Sending failed tho");
		return -1;
	}
	return 1;
}

int UDP_recieve(int socket, void* buffer, int buffer_size, sockaddr_in* senderAdress, socklen_t* senderAdressLength){
	//ssize_t receiveMessageLength;
	memset(buffer, 0, buffer_size);
	if(	recvfrom(socket, buffer, buffer_size, 0, (struct sockaddr *)senderAdress, senderAdressLength) <= 0){
		perror("Receive failed yo");
		return -1;
	}
	else{
		return 1;
	}
}

