#pragma once

#include <sys/socket.h>	// Sockets & network
#include <arpa/inet.h>	// htonl() etc.
#include <stdio.h>		// printf()
#include <string.h>		// Strlen()
#include <errno.h>		// Error handling
#include <iostream>		// Cin
#include <unistd.h>		// Pause


//const int BUF_SIZE = 1024;

#define MYPORT 30000
//int listenPort = 20013;
#define SENDPORT 30000
const char broadcastIP[] = "129.241.187.255";
//struct sockaddr_in sendAddress;

int UDP_init_socket_recieve();
int UDP_init_socket_send(sockaddr_in* sendAddress);

bool UDP_checksum(); //DUMMY!

int UDP_send(int socket, void* message, sockaddr_in* sendAddress);
int UDP_recieve(void* buffer, int buffer_size, sockaddr_in* senderAdress, socklen_t* senderAdressLength);


//bool UDP_acknowledge();
