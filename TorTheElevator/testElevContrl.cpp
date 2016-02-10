#include "elevator_controll.cpp"
#include <iostream>
#include <unistd.h>

int main(){
	elev_init();

	bool buttonMatrix[N_FLOORS][N_BUTTONS];
	while(1){
		elev_get_button_signal(buttonMatrix);
		for (int floor = 0; floor < N_FLOORS; floor++) {
        	for (ButtonType button = 0; button < N_BUTTONS; button++){
        		cout << buttonPressMatrix[floor][button] << " ";
        	}
        	cout << endl;
    	}
    	sleep(1);
	}

}
