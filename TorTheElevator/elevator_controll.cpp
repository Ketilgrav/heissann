#include "elevator_controll.h"

#define MOTOR_SPEED 2800
#define NO_FLOOR -1

static const int lamp_channel_matrix[N_FLOORS][N_BUTTONS] = {
    {LIGHT_UP1, LIGHT_DOWN1, LIGHT_COMMAND1},
    {LIGHT_UP2, LIGHT_DOWN2, LIGHT_COMMAND2},
    {LIGHT_UP3, LIGHT_DOWN3, LIGHT_COMMAND3},
    {LIGHT_UP4, LIGHT_DOWN4, LIGHT_COMMAND4},
};

static const int button_channel_matrix[N_FLOORS][N_BUTTONS] = {
    {BUTTON_UP1, BUTTON_DOWN1, BUTTON_COMMAND1},
    {BUTTON_UP2, BUTTON_DOWN2, BUTTON_COMMAND2},
    {BUTTON_UP3, BUTTON_DOWN3, BUTTON_COMMAND3},
    {BUTTON_UP4, BUTTON_DOWN4, BUTTON_COMMAND4},
};


void elev_init(void) {
    int init_success = io_init();
    assert(init_success && "Unable to initialize elevator hardware!");

    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button = 0; button < N_BUTTONS; button++){
        	io_write_bit(lamp_channel_matrix[floor][button], 0);
        }
    }

    io_clear_bit(LIGHT_STOP);
    elev_set_door_open_lamp(0);
    elev_set_floor_indicator(0);
}

void elev_set_motor_direction(MotorDirection direction) {
    if (direction == motorStop){
        io_write_analog(MOTOR, 0);
    } 
    else if (direction == motorUp) {
        io_clear_bit(MOTORDIR);
        io_write_analog(MOTOR, MOTOR_SPEED);
    } 
    else if (direction == motorDown) {
        io_set_bit(MOTORDIR);
        io_write_analog(MOTOR, MOTOR_SPEED);
    }
}


void elev_set_button_lamp(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH]) {
	for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button = 0; button < N_BUTTONS; button++){
        	io_write_bit(lamp_channel_matrix[floor][button], requestMatrix[floor][button]);
        }
    }
}

void elev_set_floor_indicator(int floor) {
    // Binary encoding. One light must always be on.
    io_write_bit(LIGHT_FLOOR_IND1,floor>>1);
	io_write_bit(LIGHT_FLOOR_IND2,floor & 0b01);
}

void elev_get_button_signal(bool buttonPressMatrix[N_FLOORS][N_BUTTONS]) {
    
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button = 0; button < N_BUTTONS; button++){
        	buttonPressMatrix[floor][button] = io_read_bit(button_channel_matrix[floor][button]);
        }
    }
}

int elev_get_floor_sensor_signal(void) {
    if (io_read_bit(SENSOR_FLOOR1)) {
        return 0;
    } else if (io_read_bit(SENSOR_FLOOR2)) {
        return 1;
    } else if (io_read_bit(SENSOR_FLOOR3)) {
        return 2;
    } else if (io_read_bit(SENSOR_FLOOR4)) {
        return 3;
    } else {
        return -1;
    }
}

void elev_set_door_open_lamp(bool value) {
	io_write_bit(LIGHT_DOOR_OPEN,value);
}