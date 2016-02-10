#pragma once

void elev_init(void);

void elev_set_motor_direction(MotorDirection direction);
void elev_set_button_lamp(const bool requestMatrix[N_FLOORS][N_BUTTONS]);
void elev_set_floor_indicator(int floor);
int elev_get_button_signal(bool buttonPressMatrix[][]);
int elev_get_floor_sensor_signal(void);