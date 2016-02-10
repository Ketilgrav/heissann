#pragma once

#include "main_include.h"

void elev_init(void);

void elev_set_motor_direction(MotorDirection direction);
void elev_update_button_lamps(const bool requestMatrix[N_FLOORS][N_BUTTONS]);
void elev_set_floor_indicator(int floor);
void elev_update_button_press(bool buttonPressMatrix[N_FLOORS][N_BUTTONS]);
int elev_get_floor_sensor_signal(void);
void elev_set_door_open_lamp(int value);

