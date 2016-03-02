#pragma once

#define N_BUTTONS 3
#define N_FLOORS 4

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