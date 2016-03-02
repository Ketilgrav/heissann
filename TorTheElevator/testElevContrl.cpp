#include "elevator_controll.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main(){
	elev_init();
	int floor;
	bool buttonMatrix[N_FLOORS][N_BUTTONS];
	while(1){
		elev_get_button_signal(buttonMatrix);
		elev_set_button_lamp(buttonMatrix);
		floor = elev_get_floor_sensor_signal();

		if(floor >=0){
			elev_set_floor_indicator(floor);
		}
		if(buttonMatrix[1][0]){
			elev_set_motor_direction(motorUp);
		}
		else if(buttonMatrix[1][1]){
			elev_set_motor_direction(motorDown);
		}
		else{
			elev_set_motor_direction(motorStop);
		}

		elev_set_door_open_lamp(buttonMatrix[0][0]);

		/*system("clear");
		for (int floor = 0; floor < N_FLOORS; floor++) {
        	for (int button = 0; button < N_BUTTONS; button++){
        		printf("%i ", buttonMatrix[floor][button]);
        	}
        	printf("\n");
    	}*/
    	

	}

}
