#define elevatorId

//Request matrisa burde ikke ligge her
#define numFloors 4
#define operatorButton 0
#define upButton 1
#define downButton 2
#define owner 3



//Motor defines
#define up 1;
#define down -1;
#define off 0

enum State {stateStartup, stateMove, stateOpenDoors, stateWait};

#define noFloor -1
#define doorOpenTime_ms 5000

void stateMachine(int& request_matrix[numFloors][4];){
    State current_state = stateStartup;
    int current_floor;
    int last_floor;
    bool move_dir = up;
    
    while(1){
        current_floor = readFloor();    //From elevator controll
        
        if(current_floor != noFloor){
            last_floor = current_floor;
        }
        
        setFloorIndicator(last_floor);  //elevator controll
        
        switch(current_state){
            case(stateStartup):
                motor.setDir(up);       //elevator controll
                if(current_floor != noFloor){
                    motor.setDir(off);
                    current_state = stateWait;
                }
            break;
            
            case(stateMove):
                motor.setDir(up);
                if(check_stop(current_floor, move_dir)){
                    current_state = stateOpenDoors;
                }
                if(reuestInDir(move_dir,last_floor)){
                    current_state = stateWait;
                }
            break;
            
            case(stateOpenDoors):
                motor.setDir(off);
                popRequest(current_floor);
                
                //Door cycle
                doors.open(1);      //elevator controll
                _delay_ms(doorOpenTime_ms);
                doors.open(0);
                current_state = stateWait;
            break;
            
            case(stateWait):
                if(requestOnFloor(current_floor)){
                    curent_state = stateOpenDoors;
                }
                else if(requestInDir(move_dir, current_floor)){
                    current_state = stateMove;
                }
                else if(requestInDir(-move_dir,current_floor)){
                    move_dir = -move_dir;
                    current_state = stateMove;
                }
            break;
            
        }
    }
}

int motorToMatrixDir(int motorDir){
    if(move_dir == down){
        return downButton;
    }
    if(move_dir == up){
        return upButton;
    }
}

bool check_stop(int current_floor,int move_dir){
    if(current_floor < 0){
        cout << "ERROR";
    }
    
    //Om noen inne i heisen vil av
    if(request_matrix[current_floor][operatorButton]){
        return true;
    }
    //om det er vår oppgave å hente noen på denne etasjoen
    else if(request_matrix[current_floor][owner]==elevatorID){
        //hvis vi uansett er på vei denne veien
        if(request_matrix[current_floor][motorToMatrixDir(move_dir)]){
            return true;
        }
        //om vi er ferdig med denne retningen, og kan ta med folk den andre veien
        else if(request_matrix[current_floor][motorToMatrixDir(-move_dir)] && !requestInDir(move_dir,current_floor)){
            return true;
        }   
    }
    return false;
}

bool requestOnFloor(int current_floor){
    if(request_matrix[current_floor][operatorButton]){
        return true;
    }
    //om det er vår oppgave å hente noen på denne etasjoen
    else if(request_matrix[current_floor][owner]==elevatorID){
        return request_matrix[current_floor][upButton] || request_matrix[current_floor][downButton]
    }
    return false;
}

bool requestInDir(int move_dir,int current_floor){
    if(move_dir==up){
        for(int floor=current_floor+1;floor<numFloors;++floor){
            if(request_matrix[floor][operatorButton] || request_matrix[floor][upButton] || request_matrix[floor][downButton]){
                return true;
            }
        }
    }
    else if(move_dir==down){
        for(int i=current_floor-1; i>=0; --i){
            if(request_matrix[floor][operatorButton] || request_matrix[floor][upButton] || request_matrix[floor][downButton]){
                return true;
            }
        }
    }
    return false;
}

bool popRequest(int current_floor){
    //kommuniserer til gouvernøren at vi har hentet passasjerene på denne etasjen
}
