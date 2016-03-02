#pragma once

#include <sys/socket.h>	// Sockets & network
#include <arpa/inet.h>	// htonl() etc.
#include <stdio.h>		// printf()
#include <string.h>		// Strlen()
#include <errno.h>		// Error handling
#include <iostream>		// Cin
#include <thread>
#include <mutex>
#include "main_include.h"

#define TIMEOUT_TIME 5;

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

	Message sendMsg;
	Message receiveMsg;
	bool newMsg;

	void UDP_init_socket_receive();
	void UDP_init_socket_send();

	bool UDP_checksum(); //DUMMY!

	bool UDP_send();
	bool UDP_receive();

public:
	NetworkMessage(int receivePort, int sendPort, char broadcastIp[]);
	void send_message(const Message& msgOut);
	void send_message(MessageType msgType, uint8_t floor, uint8_t button, uint16_t price, time_t sendTime);

	bool receive_message();
	const Message* get_message(){ return receiveMsg;}
};

