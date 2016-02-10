#include <thread>
#include <mutex>
#include <time.h>
#include <comedi.h>
#include "main_include.h"
#include "elevator_controll.h"


using namespace std;


int main(){
	bool requestMatrix[N_FLOORS][N_BUTTONS];
	bool buttonPressMatrix[N_FLOORS][N_BUTTONS];


	while(1){
		elev_update_button_press(buttonPressMatrix);

		//If a button that has not been requested is pressed, then do request
		for(int floor = 0; floor<N_FLOORS; ++floor){
			for (int button = 0; button < N_BUTTONS; ++button) {
				if(buttonPressMatrix[floor][button] != requestMatrix[floor][button]){
					//Send request
				}
			}
		}

		//if we recieve a request over network

		//If a request has timed out, then re-request




	}
}


int calculateCost(bool** requestMatrix) {

}