#include <thread>
#include <atomic>
#include <time.h>
#include <comedi.h>
#include "main_include.h"
#include "elevator_controll.h"
#include "network.h"
#include "state_machine.h"
#include <string>


using namespace std;

#define STARTUP_SLEEP_S 3

#define RECEIVE_PORT 30000
#define SEND_PORT 30000
const char BROADCAST_IP[16] = "129.241.187.255";

#define MY_FILENAME "tor.out"

int calculate_cost(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, int latestFloor) {
	int cost = abs(floor - latestFloor);

	for(int floor = 0; floor<N_FLOORS; ++floor){
		for(int button=0;button < N_BUTTONS; ++button){
			if(requestMatrix[floor][button]){
				cost ++;
				break;
			}
		}
	}
	return cost;
}

void handle_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, unsigned int externalCost, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection, int latestFloor){
	requestMatrix[floor][button] = 1;
	if(button == buttonOperator){
		requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] = 1;
	}
	else{
		int cost = calculate_cost(requestMatrix, floor, button, latestFloor);
		if(cost < externalCost){
			requestTimeoutMatrix[floor] = time(NULL) + cost*TIMEOUT_COST_SCALER;
			networkConnection->send_message(messageRequest, floor, button, cost, time(NULL));
			requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] = 1;
		}
		else{
			requestTimeoutMatrix[floor] = time(NULL) + externalCost*TIMEOUT_COST_SCALER;
			requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] = 0;
		}

	}
}

void clear_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, bool handledByThisElevator, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection){
	if(handledByThisElevator){
		for(int button=0; button<N_BUTTONS; ++button){
			requestMatrix[floor][button] = 0;
		}
		networkConnection->send_message(messageComplete, floor, buttonUp, NULL,time(NULL));
	}
	else{
		requestMatrix[floor][buttonUp] = 0;
		requestMatrix[floor][buttonDown] = 0;
	}

	requestTimeoutMatrix[floor] = TIME_INF;
}


void abort(int code){
	//Kjør en instanse til av dette programmet.
	string command ="gnome-terminal -x sh -c ./";
	command += MY_FILENAME;
	system(command);
	exit(code);
}


int main(){
	//Delayer slik at forrige instans av programmet rekker å stoppe helt.
	sleep(STARTUP_SLEEP_S);

	NetworkMessage network(RECEIVE_PORT,SEND_PORT, BROADCAST_IP);

	bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH];
	atomic<int> finishedFloor;
	atomic<int> latestFloor;
	thread state_machine_thread(state_machine, requestMatrix, finishedFloor,latestFloor);

	bool buttonPressMatrix[N_FLOORS][N_BUTTONS];

	time_t requestTimeoutMatrix[N_FLOORS];

	int tempCost;
	while(1){
		elev_get_button_signal(buttonPressMatrix);

		//If a button that has not been requested is pressed, then do request
		for(int floor = 0; floor<N_FLOORS; ++floor){
			for (int button = 0; button < N_BUTTONS; ++button) {
				if(buttonPressMatrix[floor][button] && !requestMatrix[floor][button]){
					handle_request(requestMatrix, floor, button, -1, requestTimeoutMatrix, &network, latestFloor);
				}
			}
		}

		//if we recieve a message
		if(network.receive_message()){
			if(network.get_message()->msgType == messageRequest){
				handle_request(requestMatrix, network.get_message()->floor, network.get_message()->button, network.get_message()->price, requestTimeoutMatrix,&network,latestFloor);
			}
			//We recieved a visited floor.
			else if(network.get_message()->msgType == messageComplete){
				clear_request(requestMatrix,network.get_message()->floor,network.get_message()->button,requestTimeoutMatrix,&network);
			}
		}

		//If a request has timed out, then re-request
		for(int floor=0; floor<N_FLOORS; ++floor){
			if(requestTimeoutMatrix[floor] > time(NULL)){
				//Arbitrarily gives priority on button up, will this be a problem?
				if(requestMatrix[floor][buttonUp]){
					handle_request(requestMatrix, floor, buttonUp, -1, requestTimeoutMatrix,&network,latestFloor);
				}
				else if(requestMatrix[floor][buttonDown]){
					handle_request(requestMatrix, floor, buttonDown, -1, requestTimeoutMatrix,&network,latestFloor);
				}
				else{
					//Om vi timet ut på en operator button
					//nada
					continue;
				}
			}
		}

		//If we have finished a floor
		if(finishedFloor >= 0){
			clear_request(requestMatrix,finishedFloor,1,requestTimeoutMatrix,&network);
			finishedFloor = -1;
		}

	}
}
