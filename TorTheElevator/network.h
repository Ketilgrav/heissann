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
#include "main_include.h"

#include <net/if.h>
#include <sys/ioctl.h>



const time_t MESSAGE_TIMEOUT_TIME = DOOR_OPEN_TIME_S;

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

public:
	NetworkMessage(int receivePort, int sendPort, const char broadcastIp[]);
	void send_message(const Message* msgOut);
	void send_message(MessageType msgType, uint8_t floor, uint8_t button, uint16_t price, time_t sendTime);

	bool receive_message();
	const Message* get_message(){ return &receiveMsg;}

	int get_network_id(){return networkID;}
};

