#include <sys/socket.h>	// Sockets & network
#include <arpa/inet.h>	// htonl() etc.
#include <stdio.h>		// printf()
#include <string.h>		// Strlen()
#include <errno.h>		// Error handling
#include <iostream>		// Cin
#include <unistd.h>		// Pause


const int BUF_SIZE = 1024;

int myPort = 20009;
int listenPort = 20013;
int sendPort = 20013;
int listenSock, sendSock;
ssize_t receiveMsgLength;
unsigned char buffer[BUF_SIZE];
char sendIP[] = "129.241.187.255";
char msg[22] = "What up torstein??";

int main(){

	//Initialize socket adresses
	struct sockaddr_in listenAddr;
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_port = htons(listenPort);
	listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //IP address any
	socklen_t receiveAddrLength = sizeof(listenAddr);

	struct sockaddr_in sendAddr;
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_port = htons(sendPort);
	inet_aton(sendIP, &sendAddr.sin_addr);
	socklen_t sendAddrLength = sizeof(sendAddr);

	struct sockaddr_in serverAddr;
	socklen_t serverAddrLength = sizeof(serverAddr);

	//Initialize and bind listen socket
	if( (listenSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Failed to create listenSock\n");
	}
	if (bind(listenSock, (struct sockaddr *)&listenAddr, receiveAddrLength) < 0){
		perror("Failed to bind recSocket");
		return -1;
	}

	//Initialize and set up sendSock
	if ((sendSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Socket not created\n");
		return -1;
	}
	int broadcastPermission = 1;
	if (setsockopt(sendSock, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0){
		perror("sendSock broadcast enable failed");
		return -1;
	}


	while(1){
		//Send message
		printf("Sent: %s\n", msg);
		if (sendto(sendSock, msg, strlen(msg), 0, (struct sockaddr *)&sendAddr, sendAddrLength) < 0){
				perror("Sending socket failed");
				return -1;
		}
		//receive
		receiveMsgLength = recvfrom(listenSock, buffer, BUF_SIZE, 0, (struct sockaddr *)&serverAddr, &serverAddrLength);
		printf("received: %s\n", buffer);
		printf("From IP: %s\n\n", inet_ntoa(serverAddr.sin_addr));

		//Clear message
		memset(buffer,0,sizeof(buffer));
		sleep(1);
	}
	return 0;
}
