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


#include "main_include.h"
#define BUFFER_SIZE 1024


class NetworkDataOutline{
private:
	void* data;
	size_t size;
    time_t sendTime;
public:
	void set_send_time() { sendTime =time(NULL);}
    time_t read_time() { return sendTime; }
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

	const void* sendMsg;
	void* receiveMsg;

	void UDP_init_socket_receive();
	void UDP_init_socket_send();

	bool UDP_checksum();
	void UDP_make_checksum();

	bool UDP_send();
	bool UDP_receive();

	time_t messageTimeoutTime;

public:
	NetworkMessage(int receivePort, int sendPort, const char broadcastIp[], time_t messageTimeoutTime);
	void send_message(const void* sendMsg);
	//void send_message(MessageType msgType, uint8_t floor, uint8_t button, uint16_t price, time_t sendTime);

	bool receive_message(void* receiveMsg);
	//const uint8_t* get_message(){ return receiveMsg.data;}

	int get_network_id(){return networkID;}
};

