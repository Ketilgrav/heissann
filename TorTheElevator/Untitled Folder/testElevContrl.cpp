#include "elevator_controll.h"
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(){
	elev_init();

	bool buttonMatrix[N_FLOORS][N_BUTTONS];
	while(1){
		elev_get_button_signal(buttonMatrix);
		for (int floor = 0; floor < N_FLOORS; floor++) {
        	for (int button = 0; button < N_BUTTONS; button++){
        		printf("%i ", buttonMatrix[floor][button]);
        	}
        	printf("\n");
    	}
    	sleep(1);
	}

}
