#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
//#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream> // Cin
#include <unistd.h> // Pause

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
const int BUF_SIZE = 1024;

int myPort = 20009;
int listenPort = 30000;
int sendPort = 30000;
int listenSock, sendSock;
ssize_t receiveAddrLength;

int main(){
	unsigned char buffer[BUF_SIZE];

	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(listenPort);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	socklen_t receiveAddrLength = sizeof(myaddr);

////////////////////////
	struct sockaddr_in sendAddr;
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_port = htons(sendPort);
	inet_aton("129.241.187.255", &sendAddr.sin_addr);
	socklen_t sendAddrLength = sizeof(sendAddr);
/////////////////////////////////////

	struct sockaddr_in serverAddr;
	socklen_t serverAddrLength = sizeof(serverAddr);

	if( (listenSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		/*AF_UNIX - File system pathnames. 
		AF_INET - Internet address.*/
		printf("Listensock not created\n");
	}

	if (bind(listenSock, (struct sockaddr *)&myaddr, receiveAddrLength) < 0){
		perror("Failed to bind recSocket");
		return -1;
	}
///////////////////////////
	if ((sendSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Socket not created\n");
		return -1;
	}
	int broadcastPermission = 1;
	if (setsockopt(sendSock, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0){
		perror("sendSock broadcast enable failed");
		return -1;
	}

	char msg[22] = "( ͡° ͜ʖ ͡°)";
	while(1){
		printf("Sent: %s\n", msg);
		if (sendto(sendSock, msg, strlen(msg), 0, (struct sockaddr *)&sendAddr, sendAddrLength) < 0){
				perror("Sending socket failed");
				return -1;
		}
		receiveAddrLength = recvfrom(listenSock, buffer, BUF_SIZE, 0, (struct sockaddr *)&serverAddr, &serverAddrLength);
		printf("received: %s\n", buffer);
		printf("From IP: %s\n\n", inet_ntoa(serverAddr.sin_addr));

		memset(buffer,0,sizeof(buffer));
		sleep(1);
	}
///////////////////////////////


	return 0;
}

#pragma clang diagnostic pop