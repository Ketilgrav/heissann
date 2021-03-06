#pragma once

#include <sys/socket.h>	// Sockets & network
#include <arpa/inet.h>	// htonl() etc.
#include <stdio.h>		// printf()
#include <string.h>		// Strlen()
#include <errno.h>		// Error handling
#include <iostream>
#include <unistd.h>
#include <sys/fcntl.h>

#include <net/if.h>
#include <sys/ioctl.h>

#include <time.h>


#include "main_include.h"


class NetworkConnection{
private:
	sockaddr_in sendAddress;
	
	int receiveSocket;
	int sendSocket;

	int receivePort;
	int sendPort;
	char broadcastIp[16];

	int networkID;

	void UDP_init_socket_receive();
	void UDP_init_socket_send();

	time_t messageTimeoutTime;

public:
	NetworkMessage(int receivePort, int sendPort, const char broadcastIp[], time_t messageTimeoutTime);

	void send_message(const void* sendMsg, size_t msg_size);
	bool receive_message(void* receiveMsg, size_t msg_size);

	int get_network_id(){return networkID;}
};

