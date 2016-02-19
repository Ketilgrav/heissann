#pragma once

#include <unistd.h>

#define N_BUTTONS 3
#define REQUEST_MATRIX_WIDTH (N_BUTTONS+1)
#define N_FLOORS 4

#define TIMEOUT_COST_SCALER 1
#define TIME_INF 0xffffffff



enum MotorDirection { 
    motorDown = -1,
    motorStop = 0,
    motorUp = 1
};

enum ButtonType { 
	buttonUp = 0,
    buttonDown = 1,
    buttonCommand = 2
};

enum MessageType{
	messageRequest = 0;
	messageComplete =1;
};

struct Message{
	MessageType messageType; //1bit
	uint8_t floor;
	uint8_t button;
	uint16_t price;
	time_t sendTime;
};

#define NUM_FLOORS 4
#define OPERATOR_BUTTON 0
#define UP_BUTTON 1
#define DOWN_BUTTON 2
#define IS_RESPONSIBLE 3

//Motor defines
#define UP 1
#define DOWN -1
#define OFF 0