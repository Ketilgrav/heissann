#include "event_manager.h"

using namespace std;

#define RECEIVE_PORT 20013
#define SEND_PORT 20013
const char BROADCAST_IP[16] = "129.241.187.255";

#define INTERNAL_REQUEST -1
#define TIME_INF -1
#define TIMEOUT_TIME 25

void event_manager(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], atomic<int> *finishedFloor, const atomic<int> *latestFloor){
    
    NetworkData receiveMsg;

    NetworkMessage network(RECEIVE_PORT,SEND_PORT, BROADCAST_IP,DOOR_OPEN_TIME_S+1);
    

    bool buttonPressMatrix[N_FLOORS][N_BUTTONS];
    time_t requestTimeoutMatrix[N_FLOORS];
    int calculatedCost[N_FLOORS];


    while(1){

        /******
        Button request
        ******/
        elev_get_button_signal(buttonPressMatrix);
        for(int floor = 0; floor<N_FLOORS; ++floor){
            for (int button = 0; button < N_BUTTONS; ++button) {
                if(buttonPressMatrix[floor][button] && !requestMatrix[floor][button]){
                    handle_request(requestMatrix, floor, button, INTERNAL_REQUEST, requestTimeoutMatrix, &network, *latestFloor,calculatedCost);
                }
            }
        }
        elev_set_button_lamp(requestMatrix);

        /******
        Elevator compledet request
        ******/
        if((*finishedFloor) >= 0 && (*finishedFloor)<=N_FLOORS){
            clear_request(requestMatrix,*finishedFloor,1,requestTimeoutMatrix,&network);
            (*finishedFloor) = -1;
        }

        /******
        Network request
        ******/
        if(network.receive_message(&receiveMsg)){
            if(receiveMsg.msgType == messageRequest){
                handle_request(requestMatrix, receiveMsg.floor, receiveMsg.button, receiveMsg.cost, requestTimeoutMatrix,&network,*latestFloor,calculatedCost);
            }

            else if(receiveMsg.msgType == messageComplete){
                clear_request(requestMatrix,receiveMsg.floor,false,requestTimeoutMatrix,&network);
            }
        }


        /******
        Timeout request
        ******/
        for(int floor=0; floor<N_FLOORS; ++floor){
            if(requestTimeoutMatrix[floor] < time(NULL)){
                if(requestMatrix[floor][buttonUp]){
                    handle_request(requestMatrix, floor, buttonUp, INTERNAL_REQUEST, requestTimeoutMatrix,&network,*latestFloor,calculatedCost);
                }
                else if(requestMatrix[floor][buttonDown]){
                    handle_request(requestMatrix, floor, buttonDown, INTERNAL_REQUEST, requestTimeoutMatrix,&network,*latestFloor,calculatedCost);
                }
            }
        }

    }
}





int calculate_cost(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, int latestFloor, int baseCost) {
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

    cout << cost << endl;
    return cost;
}

void handle_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, unsigned int externalCost, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection, int latestFloor,int calculatedCost[N_FLOORS]){
    requestMatrix[floor][button] = 1;
    int cost;
    //If we allready have that request, then we should not recalculate the cost, since it is probably 
    //our own message that returned to us. 
    if(requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] && requestMatrix[floor][button]){
        cost = calculatedCost[floor];
    }
    else{
        cost = calculate_cost(requestMatrix, floor, button, latestFloor, networkConnection->get_network_id());
        calculatedCost[floor] = cost;
    }

    if(button == buttonOperator){
        requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] = 1;
    }
    else{

        if(cost < externalCost){
            requestTimeoutMatrix[floor] = time(NULL) + TIMEOUT_TIME;
            NetworkData sendMsg(messageRequest,floor,button,cost);
            networkConnection->send_message(&sendMsg);

            requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] = 1;
        }
        else if(requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY]==0  || cost > externalCost){
            requestTimeoutMatrix[floor] = time(NULL) + TIMEOUT_TIME;
            requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY]=0
        }
    }
}

void clear_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, bool handledByThisElevator, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection){
    if(handledByThisElevator){
        for(int button=0; button<N_BUTTONS; ++button){
            requestMatrix[floor][button] = 0;
        }

        NetworkData sendMsg(messageComplete, floor, 0, 0);
        networkConnection->send_message(sendMsg);
        
    }
    else{
        requestMatrix[floor][buttonUp] = 0;
        requestMatrix[floor][buttonDown] = 0;
    }
    requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] = 0;
    requestTimeoutMatrix[floor] = TIME_INF;
}