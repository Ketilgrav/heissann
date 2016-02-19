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


//const int BUF_SIZE = 1024;

#define MYPORT 30000
//int listenPort = 20013;
#define SENDPORT 30000
const char broadcastIP[] = "129.241.187.255";
//struct sockaddr_in sendAddress;




//bool UDP_acknowledge();


class NetworkMessage
{
private:
	sockaddr_in sendAddress;
	//socklen_t receiveAddressLength;
	int receiveSocket;
	int sendSocket;

	Message sendMsg;
	Message receiveMsg;
	bool newMsg;

	void UDP_init_socket_recieve();
	void UDP_init_socket_send();

	bool UDP_checksum(); //DUMMY!

	void UDP_send();
	int UDP_receive();

public:
	NetworkMessage();
	void send_message(const Message& msgOut);
	void send_message(MessageType msgType, uint8_t floor, uint8_t button, uint16_t price, time_t sendTime);

	Message getNewRecieveMessage();
};

