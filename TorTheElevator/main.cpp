#include <thread>
#include <mutex>
#include <time.h>
#include <comedi.h>
#include "main_include.h"
#include "elevator_controll.h"
#include "network.h"


using namespace std;


int main(){
	UDP messageInn();
	messageInn.msg(flor, button, price, sendtype);



	bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH];
	int finishedFloor;
	mutex finishedFloorMutex;
	thread state_machine(requestMatrix,finishedFloor,finishedRequestMutex);

	bool buttonPressMatrix[N_FLOORS][N_BUTTONS];

	time_t requestTimeoutMatrix[N_FLOORS];

	message msgInn;
	mutex msgInnMutex;
	bool newMessage;
	thread network_reciever(&msgInn, &msgInnMutex, &newMessage);

	message msgOut;

	int tempCost;
	while(1){
		elev_update_button_press(buttonPressMatrix);

		//If a button that has not been requested is pressed, then do request
		for(int floor = 0; floor<N_FLOORS; ++floor){
			for (int button = 0; button < N_BUTTONS; ++button) {
				if(buttonPressMatrix[floor][button] && !requestMatrix[floor][button]){
					handle_request(requestMatrix, floor, button, -1, requestTimeoutMatrix);
				}
			}
		}

		//if we recieve a message
		if(newMessage){
			newMessage = 0;
			msgInnMutex.lock;
			//We recieved a request
			if(msgInn.messageType == messageRequest){
				handle_request(requestMatrix, msgInn.floor, msgInn.button, msgInn.price, requestTimeoutMatrix);
			}
			//We recieved a visited floor.
			else if(msgInn.messageType == messageComplete){
				clear_request(requestMatrix,floor,0,requestTimeoutMatrix);
			}
			
			msgInnMutex.unlock;
		}

		//If a request has timed out, then re-request
		for(int floor=0; floor<N_FLOORS; ++floor){
			if(requestTimeoutMatrix[floor] > time(NOW)){
				//Arbitrarily gives priority on button up, will this be a problem?
				if(requestMatrix[floor][UP_BUTTON]){
					handle_request(requestMatrix, floor, UP_BUTTON, -1, requestTimeoutMatrix);
				}
				else if(requestMatrix[floor][DOWN_BUTTON]){
					handle_request(requestMatrix, floor, DOWN_BUTTON, -1, requestTimeoutMatrix);
				}
				else{
					break;
				}
			}
		}

		//If we have finished a floor
		if(finishedFloor >= 0){
			finishedFloorMutex.lock;
			clear_request(requestMatrix,finishedFloor,1,requestTimeoutMatrix);
			finishedFloor = -1;

			finishedFloorMutex.unlock;
		}

	}
}

void handle_request(const bool requestMatrix[NUM_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, int externalCost, time_t requestTimeoutMatrix[N_FLOORS]){
	requestMatrix[floor][button] = 1;
	if(button == OPERATOR_BUTTON){
		requestMatrix[floor][IS_RESPONSIBLE] = 1;
	}
	else{
		int cost = calculateCost(requestMatrix, floor, button);
		if(cost > externalCost){
			requestTimeoutMatrix[floor] = time(NULL) + cost*TIMEOUT_COST_SCALER;

			msgOut.messageType = messageRequest;
			msgOut.floor = floor;
			msgOut.button = button;
			msgOut.price = cost;
			msgOut.sendTime = time(NULL);
			send_message(&msgOut);

			requestMatrix[floor][IS_RESPONSIBLE] = 1;
		}
		else{
			requestTimeoutMatrix[floor] = time(NULL) + externalCost*TIMEOUT_COST_SCALER;
			requestMatrix[floor][IS_RESPONSIBLE] = 0;
		}

	}
}

void clear_request(const bool requestMatrix[NUM_FLOORS][REQUEST_MATRIX_WIDTH], int floor, bool handledByThisElevator, time_t requestTimeoutMatrix[N_FLOORS]){
	if(handledByThisElevator){
		for(int button=0; button<N_BUTTONS; ++button){
			requestMatrix[finishedFloor][button] = 0;
		}

		msgOut.messageType = messageComplete;
		msgOut.floor = finishedFloor;
		msgOut.sendTime = time(NULL);
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