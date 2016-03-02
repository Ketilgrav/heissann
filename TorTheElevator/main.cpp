#include <thread>
#include <atomic>
#include <time.h>
#include <comedi.h>
#include "main_include.h"
#include "elevator_controll.h"
#include "network.h"


using namespace std;

#define RECEIVE_PORT 30000
#define SEND_PORT 30000
#define BROADCAST_IP "129.241.187.255";


int main(){
	NetworkMessage network(RECEIVE_PORT,SEND_PORT,BROADCAST_IP);
	
	bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH];
	atomic<int> finishedFloor;
	thread state_machine(requestMatrix,finishedFloor);

	bool buttonPressMatrix[N_FLOORS][N_BUTTONS];

	time_t requestTimeoutMatrix[N_FLOORS];

	int tempCost;
	while(1){
		elev_update_button_press(buttonPressMatrix);

		//If a button that has not been requested is pressed, then do request
		for(int floor = 0; floor<N_FLOORS; ++floor){
			for (int button = 0; button < N_BUTTONS; ++button) {
				if(buttonPressMatrix[floor][button] && !requestMatrix[floor][button]){
					handle_request(requestMatrix, floor, button, -1, requestTimeoutMatrix, &network);
				}
			}
		}

		//if we recieve a message
		if(network.receiveMsg()){
			if(network.receiveMsg.messageType == messageRequest){
				handle_request(requestMatrix, network.receiveMsg.floor, network.receiveMsg.button, network.receiveMsg.price, requestTimeoutMatrix);
			}
			//We recieved a visited floor.
			else if(network.receiveMsg.messageType == messageComplete){
				clear_request(requestMatrix,network.receiveMsg.floor,network.receiveMsg.button,requestTimeoutMatrix);
			}
		}

		//If a request has timed out, then re-request
		for(int floor=0; floor<N_FLOORS; ++floor){
			if(requestTimeoutMatrix[floor] > time(NULL)){
				//Arbitrarily gives priority on button up, will this be a problem?
				if(requestMatrix[floor][UP_BUTTON]){
					handle_request(requestMatrix, floor, UP_BUTTON, -1, requestTimeoutMatrix);
				}
				else if(requestMatrix[floor][DOWN_BUTTON]){
					handle_request(requestMatrix, floor, DOWN_BUTTON, -1, requestTimeoutMatrix);
				}
				else{
					//Om vi timet ut på en opertaor button
					break;
				}
			}
		}

		//If we have finished a floor
		if(finishedFloor >= 0){
			clear_request(requestMatrix,finishedFloor,1,requestTimeoutMatrix);
			finishedFloor = -1;
		}

	}
}

void handle_request(const bool requestMatrix[NUM_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, int externalCost, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection){
	requestMatrix[floor][button] = 1;
	if(button == buttonCommand){
		requestMatrix[floor][IS_RESPONSIBLE] = 1;
	}
	else{
		int cost = calculateCost(requestMatrix, floor, button);
		if(cost > externalCost){
			requestTimeoutMatrix[floor] = time(NULL) + cost*TIMEOUT_COST_SCALER;
			networkConnection.sendMessage(messageRequest, floor, button, cost, time(NULL));
			requestMatrix[floor][IS_RESPONSIBLE] = 1;
		}
		else{
			requestTimeoutMatrix[floor] = time(NULL) + externalCost*TIMEOUT_COST_SCALER;
			requestMatrix[floor][IS_RESPONSIBLE] = 0;
		}

	}
}

void clear_request(const bool requestMatrix[NUM_FLOORS][REQUEST_MATRIX_WIDTH], int floor, bool handledByThisElevator, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage networkConnection){
	if(handledByThisElevator){
		for(int button=0; button<N_BUTTONS; ++button){
			requestMatrix[finishedFloor][button] = 0;
		}
		networkMessage.send(messageComplete, finishedFloor, buttonUp, NULL,time(NULL));
	}
	else{
		requestMatrix[finishedFloor][UP_BUTTON] = 0;
		requestMatrix[finishedFloor][DOWN_BUTTON] = 0;
	}

	requestTimeoutMatrix[floor] = TIME_INF;
}

int calculateCost(const bool requestMatrix[NUM_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button) {
	//Jeg har antatt at høyere cost er bedre, dette gir ikke mening med navnet.
}
