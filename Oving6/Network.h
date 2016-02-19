#pragma once

#include <sys/socket.h>	// Sockets & network
#include <arpa/inet.h>	// htonl() etc.
#include <stdio.h>		// printf()
#include <string.h>		// Strlen()
#include <errno.h>		// Error handling
#include <iostream>		// Cin
#include <unistd.h>		// Pause


//const int BUF_SIZE = 1024;
#define TIMEOUT_SEC 1
#define TIMEOUT_USEC 500000
#define MASTER_PORT 31213
#define SLAVE_PORT 31214
const char broadcastIP[] = "129.241.187.159";
//struct sockaddr_in sendAddress;

int UDP_init_socket_recieve(int port);
int UDP_init_socket_send(sockaddr_in* sendAddress, int port);

bool UDP_checksum(); //DUMMY!

int UDP_send(int socket, void* message, sockaddr_in* sendAddress);
int UDP_recieve(int socket, void* buffer, int buffer_size, sockaddr_in* senderAdress, socklen_t* senderAdressLength);


//bool UDP_acknowledge();
