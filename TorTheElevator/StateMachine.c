//Request matrisa burde ikke ligge her
#define numFloors 4
#define operatorButton 0
#define uppButton 1
#define downButton 2
#define owner 3

int request_matrix[numFloors][4];

//
#define up 1;
#define down 0;

enum State {stateStartup, stateMove, stateOpenDoors, stateWait};

void stateMachine(){
    State current_state = stateStartup;
    int current_floor;
    int last_floor;
    bool move_dir = up;
    
    while(1){
        current_floor = readFloor();   
        switch(current_state){
            case(stateStartup):
            
            break;
            case(stateMove):
            
            break;
            case(stateOpenDoors):
            
            break;
            case(stateWait):
            
            break;
            
            
        }
        
        
        
    }
}
