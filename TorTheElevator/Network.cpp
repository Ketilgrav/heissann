#include "Network.h"

enum MessageType{
	messageRequest = 0;
	messageComplete =1;
};

struct message{
	MessageType messageType; //1bit
	uint8_t floor;
	uint8_t button;
	uint16_t price;
};


void network_recieve_thread(message* msgInn, mutex* msgInn_mutex, bool* newMessage, mutex* newMessage_mutex){
	while(1){

	}
}

void send_message(const message* msgOut){

}



int UDP_init_socket_recieve(){
	//Initialize recieve socket address
	struct sockaddr_in recieveAddress;
	recieveAddress.sin_family = AF_INET;
	recieveAddress.sin_port = htons(MYPORT);
	recieveAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	socklen_t receiveAddressLength = sizeof(recieveAddress);

	//Initialize and bind listen socket
	int recieveSocket;
	if ( (recieveSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Failed to create recieveSocket\n");
		return -1;
	}
	if (bind(recieveSocket, (struct sockaddr *)&recieveAddress, receiveAddressLength) < 0){
		perror("Failed to bind recieveSocket");
		return -1;
	}
	return recieveSocket;
}

int UDP_init_socket_send(sockaddr_in* sendAddress){
	sendAddress->sin_family = AF_INET;
	sendAddress->sin_port = htons(SENDPORT);
	inet_aton(broadcastIP, &sendAddress->sin_addr);

	//Initialize and set up sendSocket
	int sendSocket;
	if ( (sendSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
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
	if ( sendto(socket, message, 1024, 
	 0, (struct sockaddr *)sendAddress, sizeof(*sendAddress)) < 0){
		perror("Sending failed");
		return -1;
	}
	return 1;
}

int UDP_recieve(void* buffer, int buffer_size, sockaddr_in* senderAdress, socklen_t* senderAdressLength){
	int socket = UDP_init_socket_recieve();
	ssize_t receiveMessageLength;
	while(1){
		if(	(receiveMessageLength = recvfrom(socket, buffer, buffer_size,
		 0, (struct sockaddr *)senderAdress, senderAdressLength)) <= 0){
			perror("Receive failed");
		
		memset(receiveMessage, 0, 1024);
		return -1;
		}
	}
}

