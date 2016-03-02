#include "network.h"


void NetworkMessage::UDP_init_socket_recieve(){
	//Initialize recieve socket address
	sockaddr_in recieveAddress;
	recieveAddress.sin_family = AF_INET;
	recieveAddress.sin_port = htons(MYPORT);
	recieveAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	socklen_t receiveAddressLength = sizeof(recieveAddress);

	//Initialize and bind listen socket
	if ( (recieveSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Failed to create recieveSocket\n");
		sleep(2);
		exit(1);
	}
	if (bind(recieveSocket, (struct sockaddr *)&recieveAddress, receiveAddressLength) < 0){
		perror("Failed to bind recieveSocket");
		sleep(2);
		exit(1);
	}
}

void NetworkMessage::UDP_init_socket_send(){
	sendAddress.sin_family = AF_INET;
	sendAddress.sin_port = htons(SENDPORT);
	inet_aton(broadcastIP, &sendAddress.sin_addr);

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

bool NetworkMessage::UDP_checksum(){ //DUMMY!
	return true;
}

void NetworkMessage::UDP_send(){
	//hvordan sende message structen vår?
	if ( sendto(sendSocket, sendMsg, sizeof(sendMsg), 0, (struct sockaddr *)sendAddress, sizeof(*sendAddress)) < 0){
		perror("Sending failed");
	}
}

void NetworkMessage::UDP_receive(){
	//problemer med mutex. Endre til nonblocking, for deretter å polle.
	newMsg = 0;
	while(1){
		if(	(recvfrom(receiveSocket, receiveMsg, sizeof(receiveMsg), 0, nullptr, NULL)) <= 0){
			perror("Receive failed");
		}
		newMsg = 1;
	}
}

void NetworkMessage::send_message(){
	//add checksum
	//
}


NetworkMessage::NetworkMessage(){
		UDP_init_socket_recieve();
		UDP_init_socket_send();
		thread recieveThread(UDP_receive);
}

const Message* getNewRecieveMessage(){
	//Hvordan løser vi det om beskjeden endres mens vi leser den?
	if(!newMsg) return nullptr;
	else{
		newMsg = 0;
		return &receiveMsg;
	}
}