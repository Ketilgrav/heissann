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


#include "main_include.h"
#define BUFFER_SIZE 1024
enum MessageType{
	messageRequest = 0,
	messageComplete = 1
};

struct Message{
	uint8_t data[BUFFER_SIZE];
	time_t sendTime;
	uint8_t checkSum;
};

class NetworkMessage
{
private:
	sockaddr_in sendAddress;
	//socklen_t receiveAddressLength;
	int receiveSocket;
	int sendSocket;

	int receivePort;
	int sendPort;
	char broadcastIp[16];

	int networkID;

	Message sendMsg;
	Message receiveMsg;

	void UDP_init_socket_receive();
	void UDP_init_socket_send();

	bool UDP_checksum();
	void UDP_make_checksum();

	bool UDP_send();
	bool UDP_receive();

	time_t messageTimeoutTime;

public:
	NetworkMessage(int receivePort, int sendPort, const char broadcastIp[], time_t messageTimeoutTime);
	void send_message(const uint8_t* data, int size);
	//void send_message(MessageType msgType, uint8_t floor, uint8_t button, uint16_t price, time_t sendTime);

	bool receive_message();
	const uint8_t* get_message(){ return &receiveMsg.data;}

	int get_network_id(){return networkID;}
};

