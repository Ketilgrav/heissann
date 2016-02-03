#include <thread>
#include <unistd.h>

#include "Network.h"

using namespace std;

int main(){
	char receiveMessage[1024];
	sockaddr_in senderAddress;
	socklen_t senderAddressLenght;
	thread recieveThread(UDP_recieve, receiveMessage, 1024, &senderAddress, &senderAddressLenght);

	sockaddr_in sendAddress;
	int sendSocket = UDP_init_socket_send(&sendAddress);
	char sendMessage[30] = "01 Test message pls ignore.";
	while(1){
		UDP_send(sendSocket, sendMessage, &sendAddress);
		printf("%s\n", receiveMessage);
		sendMessage[1]++;
		memset(receiveMessage, 0, 1024);
		sleep(1);
	}

	return 0;
}