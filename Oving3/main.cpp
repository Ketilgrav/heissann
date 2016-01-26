#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
//#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream> // Cin
#include <unistd.h> // Pause

const int BUF_SIZE = 1024;

int myPort = 20009;
int listenPort = 30000;
int sendPort = 30000;
int listenSock, sendSock;
ssize_t recieveMsgLenght;

int main(){
	unsigned char buffer[BUF_SIZE];

	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(listenPort);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	socklen_t recieveAddrLenght = sizeof(myaddr);

////////////////////////
	struct sockaddr_in sendAddr;
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_port = htons(sendPort);
	inet_aton("129.241.187.255", &sendAddr.sin_addr);
	socklen_t sendAddrLenght = sizeof(sendAddr);
/////////////////////////////////////

	struct sockaddr_in serverAddr;
	socklen_t serverAddrLength = sizeof(serverAddr);

	if( (listenSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		/*AF_UNIX - File system pathnames. 
		AF_INET - Internet address.*/
		printf("Listensock not created\n");
	}

	if (bind(listenSock, (struct sockaddr *)&myaddr, recieveAddrLenght) < 0){
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

	char msg[22] = "HILSER FRA LABPLASS 9";
	while(1){
		/*printf("Sent: %s\n", msg);
		if (sendto(sendSock, msg, strlen(msg), 0, (struct sockaddr *)&sendAddr, sendAddrLenght) < 0){
				perror("Sending socket failed");
				return -1;
		}*/
		recieveMsgLenght = recvfrom(listenSock, buffer, BUF_SIZE, 0, (struct sockaddr *)&serverAddr, &serverAddrLength);
		printf("Recieved: %s\n", buffer);
		printf("From IP: %s\n\n", inet_ntoa(serverAddr.sin_addr));

		memset(buffer,0,sizeof(buffer));
		sleep(1);
	}
///////////////////////////////


	return 0;
}