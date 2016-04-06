#pragma once

#define N_BUTTONS 3
#define N_FLOORS 4
#define REQUEST_MATRIX_WIDTH 4

#include <stdint.h>
#include <time.h>

enum MotorDirection { 
    motorDown = -1,
    motorStop = 0,
    motorUp = 1
};

enum ButtonType { 
	buttonUp = 0,
    buttonDown = 1,
    buttonOperator = 2
};
#define REQUEST_MATRIX_RESPONSIBILITY 3

enum MessageType{
	messageRequest = 0,
	messageComplete = 1
};

struct Message{
	MessageType msgType;
	uint8_t floor;
	uint8_t button;
	uint16_t price;
	time_t sendTime;
	uint8_t checkSum;
};

#define TIME_INF -1
#define TIMEOUT_COST_SCALER 1

#define DOOR_OPEN_TIME_S 1
#define DOOR_OPEN_TIME_uS DOOR_OPEN_TIME_S*1000000

//int calculate_cost(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, int latestFloor);
//void handle_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, unsigned int externalCost, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection, int latestFloor);
//void clear_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, bool handledByThisElevator, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection);

void abort(int code);