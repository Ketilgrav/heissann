#include "event_manager.h"
#include "elevator_controll.h"

using namespace std;

#define RECEIVE_PORT 20013
#define SEND_PORT 20013
const char BROADCAST_IP[16] = "129.241.187.255";


void event_manager(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], atomic<int> *finishedFloor,  atomic<int> *latestFloor){
    NetworkMessage network(RECEIVE_PORT,SEND_PORT, BROADCAST_IP);
    

    bool buttonPressMatrix[N_FLOORS][N_BUTTONS];
    time_t requestTimeoutMatrix[N_FLOORS];
    int calculatedCost[N_FLOORS];

    //int tempCost;
    while(1){
        elev_get_button_signal(buttonPressMatrix);

        //If a button that has not been requested is pressed, then do request
        for(int floor = 0; floor<N_FLOORS; ++floor){
            for (int button = 0; button < N_BUTTONS; ++button) {
                if(buttonPressMatrix[floor][button] && !requestMatrix[floor][button]){
                    handle_request(requestMatrix, floor, button, -1, requestTimeoutMatrix, &network, *latestFloor,calculatedCost);
                }
            }
        }
        elev_set_button_lamp(requestMatrix);

        //if we recieve a message
        if(network.receive_message()){
            if(network.get_message()->msgType == messageRequest){
                handle_request(requestMatrix, network.get_message()->floor, network.get_message()->button, network.get_message()->price, requestTimeoutMatrix,&network,*latestFloor,calculatedCost);
            }
            //We recieved a visited floor.
            else if(network.get_message()->msgType == messageComplete){
                clear_request(requestMatrix,network.get_message()->floor,network.get_message()->button,requestTimeoutMatrix,&network);
            }
        }


        //If a request has timed out, then re-request
        for(int floor=0; floor<N_FLOORS; ++floor){
            if(requestTimeoutMatrix[floor] < time(NULL)){
                //Arbitrarily gives priority on button up, will this be a problem?
                if(requestMatrix[floor][buttonUp]){
                    handle_request(requestMatrix, floor, buttonUp, -1, requestTimeoutMatrix,&network,*latestFloor,calculatedCost);
                }
                else if(requestMatrix[floor][buttonDown]){
                    handle_request(requestMatrix, floor, buttonDown, -1, requestTimeoutMatrix,&network,*latestFloor,calculatedCost);
                }
                else{
                    //Om vi timet ut på en operator button
                    //nada
                    continue;
                }
            }
        }

        //If we have finished a floor
        if((*finishedFloor) >= 0 && (*finishedFloor)<=N_FLOORS){
            clear_request(requestMatrix,*finishedFloor,1,requestTimeoutMatrix,&network);
            (*finishedFloor) = -1;
        }

    }
}





int calculate_cost(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, int latestFloor, int baseCost) {
    int cost = floor - latestFloor;
    cost = cost < 0 ? -cost : cost;


    /*for(int floor = 0; floor<N_FLOORS; ++floor){
        for(int button=0;button < N_BUTTONS; ++button){
            if(requestMatrix[floor][button]){
                cost ++;
                break;
            }
        }
    }*/

    cost *= 256;
    cost += baseCost; //Om to heiser har lik kost, ekspeders oppgaven av den med høyest siste byte i IP-adressen

    cout << cost;
    cout << cost << endl;
    return cost;
}

void handle_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, int button, unsigned int externalCost, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection, int latestFloor,int calculatedCost[N_FLOORS]){
    requestMatrix[floor][button] = 1;
    int cost;
    if(requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY]){
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
            networkConnection->send_message(messageRequest, floor, button, cost, time(NULL));
            requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] = 1;
        }
        else if(requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY]==0  || cost > externalCost){
            requestTimeoutMatrix[floor] = time(NULL) + TIMEOUT_TIME;
            requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] = 0;
        }
    }
}

void clear_request(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int floor, bool handledByThisElevator, time_t requestTimeoutMatrix[N_FLOORS], NetworkMessage* networkConnection){
    if(handledByThisElevator){
        for(int button=0; button<N_BUTTONS; ++button){
            requestMatrix[floor][button] = 0;
        }
        networkConnection->send_message(messageComplete, floor, buttonUp, 0, time(NULL));
    }
    else{
        requestMatrix[floor][buttonUp] = 0;
        requestMatrix[floor][buttonDown] = 0;
    }
    requestMatrix[floor][REQUEST_MATRIX_RESPONSIBILITY] = 0;
    requestTimeoutMatrix[floor] = TIME_INF;
}
