#include <thread>
#include <mutex>
#include <time.h>
#include <comedi.h>
#include "main_include.h"
#include "elevator_controll.h"
#include "network.h"


using namespace std;


int main(){
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
					requestMatrix[floor][button] = 1;
					requestMatrix[floor][IS_RESPONSIBLE] = 1;

					if(button != OPERATOR_BUTTON){
						tempCost = calculateCost(requestMatrix, floor, button);
						requestTimeoutMatrix[floor] = time(NULL) + tempCost*TIMEOUT_COST_SCALER;
						msgOut.messageType = messageRequest;
						msgOut.floor = floor;
						msgOut.button = button;
						msgOut.price = tempCost;
						msgOut.sendTime = time(NULL);

						send_message(&msgOut);
					}
				}
			}
		}

		//if we recieve a message
		if(newMessage){
			newMessage = 0;
			msgInnMutex.lock;
			//We recieved a request
			if(msgInn.messageType == messageRequest){
				tempCost = calculateCost(requestMatrix, msgInn.floor, msgInn.button);
				requestMatrix[floor][button] = 1;

				if(tempCost > msgInn.price){
					requestTimeoutMatrix[floor] = time(NULL) + tempCost*TIMEOUT_COST_SCALER;

					msgOut.messageType = messageRequest;
					msgOut.floor = floor;
					msgOut.button = button;
					msgOut.price = tempCost;
					msgOut.sendTime = time(NULL);
					send_message(&msgOut);

					requestMatrix[floor][IS_RESPONSIBLE] = 1;
				}
				else{
					requestTimeoutMatrix[floor] = time(NULL) + msgInn.price*TIMEOUT_COST_SCALER;

					requestMatrix[floor][IS_RESPONSIBLE] = 0;
				}
			}
			//We recieved a visited floor.
			else if(msgInn.messageType == messageComplete){
				//NB, important not to clear the opperator button
				requestMatrix[finishedFloor][UP_BUTTON] = 0;
				requestMatrix[finishedFloor][DOWN_BUTTON] = 0;
				requestTimeoutMatrix[floor] = TIME_INF;
			}
			
			msgInnMutex.unlock;
		}

		//If a request has timed out, then re-request
		for(int floor=0; floor<N_FLOORS; ++floor){
			if(requestTimeoutMatrix[floor] > time(NOW)){
				//Arbitrarily gives priority on button up, will this be a problem?
				int button;
				if(requestMatrix[floor][UP_BUTTON]){
					button = UP_BUTTON;
				}
				else if(requestMatrix[floor][DOWN_BUTTON]){
					button = DOWN_BUTTON;
				}
				else{
					break;
				}

				requestMatrix[floor][IS_RESPONSIBLE] = 1;

				tempCost = calculateCost(requestMatrix, floor, button);
				requestTimeoutMatrix[floor] = time(NULL) + tempCost*TIMEOUT_COST_SCALER;

				msgOut.messageType = messageRequest;
				msgOut.floor = floor;
				msgOut.button = button;
				msgOut.price = tempCost;
				msgOut.sendTime = time(NULL);
				send_message(&msgOut);
			}
		}

		//If we have finished a floor
		if(finishedFloor >= 0){
			finishedFloorMutex.lock;

			for(int button=0; button<N_BUTTONS; ++button){
				requestMatrix[finishedFloor][button] = 0;
			}

			msgOut.messageType = messageComplete;
			msgOut.floor = finishedFloor;
			msgOut.sendTime = time(NULL);

			requestTimeoutMatrix[floor] = TIME_INF;

			finishedFloor = -1;

			finishedFloorMutex.unlock;
		}

	}
}



int calculateCost(const bool requestMatrix[NUM_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button) {
	//Jeg har antatt at høyere cost er bedre, dette gir ikke mening med navnet.
}