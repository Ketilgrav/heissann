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
