#pragma once
#include "io.h"
#include "channels.h"
#include <assert.h>
#include "main_include.h"

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

void elev_init(void);

void elev_set_motor_direction(MotorDirection direction);
void elev_set_button_lamp(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH]);
void elev_set_floor_indicator(int floor);
void elev_get_button_signal(bool buttonPressMatrix[N_FLOORS][N_BUTTONS]);
int elev_get_floor_sensor_signal(void);
void elev_set_door_open_lamp(bool value);