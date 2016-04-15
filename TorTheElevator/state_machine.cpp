#include "state_machine.h"

using namespace std;

enum State {stateStartup, stateMove, stateOpenDoors, stateWait};

#define FLOOR_ARRIVAL_TIMEOUT 10
#define NO_FLOOR -1

void state_machine(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], atomic<int>* finishedRequest, atomic<int>* latestFloor){
    State currentState = stateStartup;
	bool atFloor;
    MotorDirection moveDir = motorUp;

    time_t time_since_floor = 0;
    
    while(1){
		int floorReadout = elev_get_floor_sensor_signal();
		if (floorReadout != NO_FLOOR) {
            (*latestFloor) = floorReadout;
			atFloor = 1;
		}
		else {
			atFloor = 0;
		}

		elev_set_floor_indicator(*latestFloor);
        
        switch(currentState){
            case(stateStartup):
				elev_set_motor_direction(moveDir);
                if(atFloor){
					elev_set_motor_direction(motorStop);
                    currentState = stateWait;
                }
            break;

			case(stateWait):
				if (atFloor && request_on_floor(requestMatrix,*latestFloor)) {
					currentState = stateOpenDoors;
				}
                else if(!atFloor && request_on_floor(requestMatrix,*latestFloor)){
                    moveDir = MotorDirection(-moveDir);
                    currentState = stateMove;
                    time_since_floor = time(NULL);
                }
				else if (request_in_dir(requestMatrix, moveDir, *latestFloor)) {
					currentState = stateMove;
                    time_since_floor = time(NULL);
				}
				else if (request_in_dir(requestMatrix,-moveDir, *latestFloor)) {
					moveDir = MotorDirection(-moveDir) ;
					currentState = stateMove;
                    time_since_floor = time(NULL);
				}
			break;

			case(stateMove):
				elev_set_motor_direction(moveDir);
                if(atFloor && check_stop(requestMatrix,*latestFloor, moveDir)){
                    currentState = stateOpenDoors;
                }
                if(atFloor && !request_in_dir(requestMatrix,moveDir,*latestFloor)){
					elev_set_motor_direction(motorStop);
                    currentState = stateWait;
                }
                /*If the elevator fails at arriving at a floor within a certain time
                it will assume motor fail and shut down (call maintainance).*/
                if(time_since_floor + FLOOR_ARRIVAL_TIMEOUT < time(NULL)){
                    elev_set_motor_direction(motorStop);
                    system("pkill executable");
                }
            break;

            case(stateOpenDoors):
				elev_set_motor_direction(motorStop);
                finishedRequest->store(*latestFloor);

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
    else{
        cout << "Tried to convert motor stop to button"<<endl;
        sleep(3);
        exit(1);
    }
}

bool check_stop(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int currentFloor,int moveDir){

    if(requestMatrix[currentFloor][buttonOperator]){
        return true;
    }
    else if(requestMatrix[currentFloor][REQUEST_MATRIX_RESPONSIBILITY]){
        if(requestMatrix[currentFloor][motor_dir_to_matrix_dir(moveDir)]){
            return true;
        }
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
    else if(requestMatrix[currentFloor][REQUEST_MATRIX_RESPONSIBILITY]){
        return requestMatrix[currentFloor][buttonUp] || requestMatrix[currentFloor][buttonDown];
    }
    return false;
}

bool request_in_dir(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int moveDir,int currentFloor){
    if(moveDir==motorUp){
        for(int floor=currentFloor+1;floor<N_FLOORS;++floor){
            if(requestMatrix[floor][buttonOperator] || (requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] && (requestMatrix[floor][buttonUp] || requestMatrix[floor][buttonDown]) )){
                return true;
            }
        }
    }
    else if(moveDir==motorDown){
        for(int floor=currentFloor-1; floor>=0; --floor){
            if(requestMatrix[floor][buttonOperator] || (requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] && (requestMatrix[floor][buttonUp] || requestMatrix[floor][buttonDown]) )){
                return true;
            }
        }
    }
    return false;
}
