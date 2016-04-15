#pragma once
#include <atomic>
#include <stdlib.h>

#include "main_include.h"
#include "network.h"
#include "elevator_controll.h"

void event_manager(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH],std::atomic<int> *finishedFloor, const std::atomic<int> *latestFloor);
int calculate_cost(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, int latestFloor,int baseCost);
void handle_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, unsigned int externalCost, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection, int latestFloor);
void clear_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, bool handledByThisElevator, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection);

#define messageComplete 0
#define messageRequest 1

class NetworkData{
public:
    uint8_t msgType;
    uint8_t floor;
    uint8_t button;
    uint16_t cost;
    uint8_t senderIp;
    NetworkData(uint8_t msgType, uint8_t floor, uint8_t button, uint16_t cost, uint8_t senderIp){
    	this->msgType = msgType;
    	this->floor = floor;
    	this->button = button;
    	this->cost = cost;
        this->senderIp = senderIp;
    }
    NetworkData(){
        msgType = messageRequest;
        floor = 0;
        button = 0;
        cost = 0;
        senderIp = 0;
    }
};
