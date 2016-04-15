#pragma once
#include <atomic>
#include <stdlib.h>

#include "main_include.h"
#include "network.h"
#include "elevator_controll.h"

void event_manager(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH],std::atomic<int> *finishedFloor, const std::atomic<int> *latestFloor);
int calculate_cost(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, int latestFloor,int cost);
void handle_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, unsigned int externalCost, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection, int latestFloor,int calculatedCost[N_FLOORS]);
void clear_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, bool handledByThisElevator, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection);

enum MessageType{
    messageRequest = 0,
    messageComplete = 1
};

class NetworkData: public NetworkDataOutline{
public:
    MessageType msgType;
    uint8_t floor;
    uint8_t button;
    uint16_t cost;
    bool calc_checksum(){
    	return 255 - (msgType+floor+button+cost)%255;
    }
};
