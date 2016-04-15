#include "event_manager.h"

using namespace std;

#define RECEIVE_PORT 20013
#define SEND_PORT 20013
const char BROADCAST_IP[16] = "129.241.187.255";

#define INTERNAL_REQUEST -1
#define TIME_INF -1
#define TIMEOUT_TIME 25

void event_manager(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], atomic<int> *finishedFloor, const atomic<int> *latestFloor){
    
    NetworkMessage network(RECEIVE_PORT,SEND_PORT, BROADCAST_IP,DOOR_OPEN_TIME_S+1);

    NetworkData receiveMsg;
    

    bool buttonPressMatrix[N_FLOORS][N_BUTTONS];
    time_t requestTimeoutMatrix[N_FLOORS];


    while(1){
        elev_set_button_lamp(requestMatrix);


        /******
        Button request
        ******/
        elev_get_button_signal(buttonPressMatrix);
        for(int floor = 0; floor<N_FLOORS; ++floor){
            for (int button = 0; button < N_BUTTONS; ++button) {
                if(buttonPressMatrix[floor][button] && !requestMatrix[floor][button]){
                    handle_request(requestMatrix, floor, button, INTERNAL_REQUEST, requestTimeoutMatrix, &network, *latestFloor);
                }
            }
        }

        /******
        Elevator completed request
        ******/
        if((*finishedFloor) >= 0 && (*finishedFloor)<=N_FLOORS){
            clear_request(requestMatrix,*finishedFloor,1,requestTimeoutMatrix,&network);
            (*finishedFloor) = -1;
        }

        /******
        Network request
        ******/
        if(network.receive_message(&receiveMsg, sizeof(NetworkData))){
            if(receiveMsg.senderIp != network.get_network_id()){
                if(receiveMsg.msgType == messageRequest){
            	    handle_request(requestMatrix, receiveMsg.floor, receiveMsg.button, receiveMsg.cost, requestTimeoutMatrix,&network,*latestFloor);
            	}

            	else if(receiveMsg.msgType == messageComplete){
	                clear_request(requestMatrix,receiveMsg.floor,false,requestTimeoutMatrix,&network);
            	}
            }
        }


        /******
        Timeout request
        ******/
        for(int floor=0; floor<N_FLOORS; ++floor){
            if(requestTimeoutMatrix[floor] < time(NULL)){
                if(requestMatrix[floor][buttonUp]){
                    handle_request(requestMatrix, floor, buttonUp, INTERNAL_REQUEST, requestTimeoutMatrix,&network,*latestFloor);
                }
                else if(requestMatrix[floor][buttonDown]){
                    handle_request(requestMatrix, floor, buttonDown, INTERNAL_REQUEST, requestTimeoutMatrix,&network,*latestFloor);
                }
            }
        }
    }
}


int calculate_cost(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, int latestFloor, int baseCost){
    int cost = abs(floor - latestFloor);

    for(int floor = 0; floor<N_FLOORS; ++floor){
        if(requestMatrix[floor][buttonOperator]){
            cost++;
        }
        else if(requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] && (requestMatrix[floor][buttonUp] || requestMatrix[floor][buttonDown])){
            cost++;
        }
    }

    cost *= 255;
    //The elevator with the lowest IP adress will handle the request if they calculate the same cost
    cost += baseCost; 
    return cost;
}

void handle_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, unsigned int externalCost, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection, int latestFloor){
    requestMatrix[floor][button] = 1;
    int cost = calculate_cost(requestMatrix, floor, button, latestFloor, networkConnection->get_network_id());
    if(button != buttonOperator){
        requestTimeoutMatrix[floor] = time(NULL) + TIMEOUT_TIME;
        if(cost < externalCost){
            NetworkData sendMsg(messageRequest,floor,button,cost, networkConnection->get_network_id());
            networkConnection->send_message(&sendMsg,sizeof(sendMsg));

            requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] = 1;
        }
        else if(cost > externalCost){
            requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY]=0;
        }
    }
}

void clear_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, bool handledByThisElevator, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection){
    if(handledByThisElevator){
        for(int button=0; button<N_BUTTONS; ++button){
            requestMatrix[floor][button] = 0;
        }
        NetworkData sendMsg(messageComplete, floor, 0, 0, networkConnection->get_network_id());
        networkConnection->send_message(&sendMsg,sizeof(sendMsg));
        
    }
    else{
        requestMatrix[floor][buttonUp] = 0;
        requestMatrix[floor][buttonDown] = 0;
    }
    requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] = 0;
    requestTimeoutMatrix[floor] = TIME_INF;
}