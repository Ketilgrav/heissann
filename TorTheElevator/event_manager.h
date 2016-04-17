#pragma once
#include <atomic>
#include <stdlib.h>

#include "main_include.h"
#include "network.h"
#include "elevator_controll.h"

void event_manager(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH],std::atomic<int> *finishedFloor, const std::atomic<int> *latestFloor);
void handle_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, unsigned int externalCost, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection, int latestFloor);
void clear_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, bool handledByThisElevator, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection);
int calculate_cost(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, int latestFloor,int baseCost);

#define MESSAGE_COMPLETE 0
#define MESSAGE_REQUEST 1

class NetworkMessage{
public:
    uint8_t msgType;
    uint8_t floor;
    uint8_t button;
    uint16_t cost;
    uint8_t senderIp;
    NetworkMessage(uint8_t msgType, uint8_t floor, uint8_t button, uint16_t cost, uint8_t senderIp){
    	this->msgType = msgType;
    	this->floor = floor;
    	this->button = button;
    	this->cost = cost;
        this->senderIp = senderIp;
    }
    NetworkMessage(){
        msgType = MESSAGE_REQUEST;
        floor = 0;
        button = 0;
        cost = 0;
        senderIp = 0;
    }
};
