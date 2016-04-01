#include "state_machine.h"

using namespace std;



enum State {stateStartup, stateMove, stateOpenDoors, stateWait};

//State machine leser fra requestMatrix, og kommuniserer tilbake gjennom finishRequest og latestFloor.
void state_machine(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], atomic<int>& finishedRequest, atomic<int>& latestFloor){
    State currentState = stateStartup;
	bool atFloor;
    MotorDirection moveDir = motorUp;
    
    while(1){
		int floorReadout = elev_get_floor_sensor_signal();
		if (floorReadout != NO_FLOOR) {
			latestFloor = floorReadout;
			atFloor = 1;
		}
		else {
			atFloor = 0;
		}

		elev_set_floor_indicator(latestFloor);
        elev_set_button_lamp(requestMatrix);
        
        switch(currentState){
            case(stateStartup):
				elev_set_motor_direction(moveDir);
                if(atFloor){
					elev_set_motor_direction(motorStop);
                    currentState = stateWait;
                }
            break;

			case(stateWait):
				if (atFloor && request_on_floor(requestMatrix,latestFloor)) {
					currentState = stateOpenDoors;
				}
				else if (request_in_dir(requestMatrix, moveDir, latestFloor)) {
					currentState = stateMove;
				}
				else if (request_in_dir(requestMatrix,-moveDir, latestFloor)) {
					moveDir *= -1;
					currentState = stateMove;
				}
			break;

			case(stateMove) :
				elev_set_motor_direction(moveDir);
                if(atFloor && check_stop(requestMatrix,latestFloor, moveDir)){
                    currentState = stateOpenDoors;
                }
                if(!request_in_dir(requestMatrix,moveDir,latestFloor)){
					elev_set_motor_direction(motorStop);
                    currentState = stateWait;
                }
            break;

            case(stateOpenDoors):
				elev_set_motor_direction(motorStop);
				finishedRequest = latestFloor;

                //Door cycle
				elev_set_door_open_lamp(1);
                sleep(DOOR_OPEN_TIME_S);
				elev_set_door_open_lamp(0);
                currentState = stateWait;
            break;

        }
    }
}

int motor_dir_to_matrix_dir(int motorDir){
    if(motorDir == motorDown){
        return buttonDown;
    }
    if(motorDir == motorUp){
        return buttonUp;
    }
}

bool check_stop(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int currentFloor,int moveDir){

    //Om noen inne i heisen vil av
    if(requestMatrix[currentFloor][buttonOperator]){
        return true;
    }
    //om det er vår oppgave å hente noen på denne etasjoen
    else if(requestMatrix[currentFloor][REQUEST_MATRIX_RESPONSIBILITY]){
        //hvis vi uansett er på vei denne veien
        if(requestMatrix[currentFloor][motor_dir_to_matrix_dir(moveDir)]){
            return true;
        }
        //om vi er ferdig med denne retningen, og kan ta med folk den andre veien
        else if(!request_in_dir(requestMatrix,moveDir, currentFloor) && requestMatrix[currentFloor][motor_dir_to_matrix_dir(-moveDir)]){
            return true;
        }
    }
    return false;
}

bool request_on_floor(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int currentFloor){
    if(requestMatrix[currentFloor][buttonOperator]){
        return true;
    }
    //om det er vår oppgave å hente noen på denne etasjoen
    else if(requestMatrix[currentFloor][REQUEST_MATRIX_RESPONSIBILITY]){
        return requestMatrix[currentFloor][buttonUp] || requestMatrix[currentFloor][buttonDown]
    }
    return false;
}

bool request_in_dir(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int moveDir,int currentFloor){
    if(moveDir==motorUp){
        for(int floor=currentFloor+1;floor<N_FLOORS;++floor){
            if(requestMatrix[floor][buttonOperator] || requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] && (requestMatrix[floor][buttonUp] || requestMatrix[floor][buttonDown])){
                return true;
            }
        }
    }
    else if(moveDir==motorDown){
        for(int floor=currentFloor-1; floor>=0; --floor){
            if(requestMatrix[floor][buttonOperator] || requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] && (requestMatrix[floor][buttonUp] || requestMatrix[floor][buttonDown])){
                return true;
            }
        }
    }
    return false;
}
