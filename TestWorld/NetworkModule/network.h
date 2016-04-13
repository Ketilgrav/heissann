#pragma once

#include <sys/socket.h>	// Sockets & network
#include <arpa/inet.h>	// htonl() etc.
#include <stdio.h>		// printf()
#include <string.h>		// Strlen()
#include <errno.h>		// Error handling
#include <iostream>		// Cin
#include <thread>
#include <mutex>
#include <unistd.h>
#include <sys/fcntl.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <time.h>

struct Message{
	void* data;
	uint8_t checksum;
	time_t sendTime;
	Message(void* data){
		this->data = data;
	}
};

class NetworkMessage{
private:
	sockaddr_in sendAddress;
	//socklen_t receiveAddressLength;
	int receiveSocket;
	int sendSocket;

	time_t messageTimeoutTime;

	int receivePort;
	int sendPort;
	char broadcastIp[16];

	int networkID;

	Message sendMsg;
	Message receiveMsg;

	void UDP_init_socket_receive();
	void UDP_init_socket_send();

	bool UDP_send();
	bool UDP_receive();

	bool UDP_checksum();
	void UDP_make_checksum();

public:
	NetworkMessage(int receivePort, int sendPort, const char broadcastIp[], time_t messageTimeoutTime);
	void send_message(void* data);
	//void send_message(MessageType msgType, uint8_t floor, uint8_t button, uint16_t price, time_t sendTime);

	bool receive_message(void* data);
	void* get_message(){return receiveMsg.data; }

	int get_network_id(){return networkID;}
};

