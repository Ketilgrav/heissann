#include <thread>
#include <mutex>

using namespace std;



enum State {stateStartup, stateMove, stateOpenDoors, stateWait};

#define NO_FLOOR -1
#define DOOR_OPEN_TIME_MS 5000

//State machine leser fra requestMatrix, og kommuniserer tilbake gjennom finishREquest.
void state_machine(const bool requestMatrix[NUM_FLOORS][REQUEST_MATRIX_WIDTH], atomic<int>& finishedRequest){
    State currentState = stateStartup;
	bool atFLoor;
    int latestFloor;
    bool moveDir = UP; //NB move dir må være 1 eller -1, ikke 0. Altså siste retning vi gikk i.
    
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
        
        switch(currentState){
            case(stateStartup):
				elev_set_motor_direction(UP); 
                if(atFloor){
					elev_set_motor_direction(OFF);
                    currentState = stateWait;
                }
            break;

			case(stateWait):
				if (request_on_floor(currentFloor)) {
					curent_state = stateOpenDoors;
				}
				else if (request_in_dir(moveDir, currentFloor)) {
					currentState = stateMove;
				}
				else if (request_in_dir(-moveDir, currentFloor)) {
					moveDir *= -1;
					currentState = stateMove;
				}
			break;
            
			case(stateMove) :
				elev_set_motor_direction(moveDir);
                if(atFloor && check_stop(currentFloor, moveDir)){
                    currentState = stateOpenDoors;
                }
                if(!requestInDir(moveDir,latestFloor)){
					elev_set_motor_direction(OFF);
                    currentState = stateWait;
                }
            break;
            
            case(stateOpenDoors):
				elev_set_motor_direction(OFF);
				
				finishedRequest = latestFloor;
                
                //Door cycle
				elev_set_door_open_lamp(1);
                _delay_ms(DOOR_OPEN_TIME_MS);
				elev_set_door_open_lamp(0);
                currentState = stateWait;
            break;
           
            
        }
    }
}

int motor_dir_to_matrix_dir(int motorDir){
    if(moveDir == DOWN){
        return DOWN_BUTTON;
    }
    if(moveDir == UP){
        return UP_BUTTON;
    }
}

bool check_stop(const bool requestMatrix[NUM_FLOORS][REQUEST_MATRIX_WIDTH], int currentFloor,int moveDir){
  
    //Om noen inne i heisen vil av
    if(requestMatrix[currentFloor][OPERATOR_BUTTON]){
        return true;
    }
    //om det er vår oppgave å hente noen på denne etasjoen
    else if(requestMatrix[currentFloor][OWNER]==elevatorID){
        //hvis vi uansett er på vei denne veien
        if(requestMatrix[currentFloor][motor_dir_to_matrix_dir(moveDir)]){
            return true;
        }
        //om vi er ferdig med denne retningen, og kan ta med folk den andre veien
        else if(!request_in_dir(moveDir, currentFloor) && requestMatrix[currentFloor][motor_dir_to_matrix_dir(-moveDir)]){
            return true;
        }   
    }
    return false;
}

bool request_on_floor(const bool requestMatrix[NUM_FLOORS][REQUEST_MATRIX_WIDTH], int currentFloor){
    if(requestMatrix[currentFloor][OPERATOR_BUTTON]){
        return true;
    }
    //om det er vår oppgave å hente noen på denne etasjoen
    else if(requestMatrix[currentFloor][IS_RESPONSIBLE]){
        return requestMatrix[currentFloor][UP_BUTTON] || requestMatrix[currentFloor][DOWN_BUTTON]
    }
    return false;
}

bool request_in_dir(const bool requestMatrix[NUM_FLOORS][REQUEST_MATRIX_WIDTH], int moveDir,int currentFloor){
    if(moveDir==UP){
        for(int floor=currentFloor+1;floor<NUM_FLOORS;++floor){
            if(requestMatrix[floor][OPERATOR_BUTTON] || requestMatrix[floor][IS_RESPONSIBLE] && (requestMatrix[floor][UP_BUTTON] || requestMatrix[floor][DOWN_BUTTON])){
                return true;
            }
        }
    }
    else if(moveDir==DOWN){
        for(int i=currentFloor-1; i>=0; --i){
            if(requestMatrix[floor][OPERATOR_BUTTON] || requestMatrix[floor][IS_RESPONSIBLE] && (requestMatrix[floor][UP_BUTTON] || requestMatrix[floor][DOWN_BUTTON])){
                return true;
            }
        }
    }
    return false;
}
