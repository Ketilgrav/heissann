#include <thread>
#include <atomic>
#include <time.h>
#include <comedi.h>
#include "main_include.h"
#include "elevator_controll.h"
#include "network.h"
#include "state_machine.h"
#include <string>


using namespace std;

#define STARTUP_SLEEP_S 3

#define RECEIVE_PORT 20013
#define SEND_PORT 20013
const char BROADCAST_IP[16] = "129.241.187.255";

#define MY_FILENAME "tor.out"



int main(){

	bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH];
    init(requestMatrix);

    atomic<int> finishedFloor(-1);
    atomic<int> latestFloor;
    thread state_machine_thread(state_machine, requestMatrix, &finishedFloor, &latestFloor);





    int i = 0;
    bool isMaster = 0;

    uint32_t sendMessage;

    while(1){
        if(isMaster) {
            sendMessage = MESSAGE_MAKE(MESSAGE_TYPE_I,i);
            if (UDP_send(sendSocket, &sendMessage, &sendAddress) == -1){
            	cout << "Failed to send as Master! of disaster\n";
            }

            buffer = 0;

            if(UDP_recieve(receiveSocket, &buffer, sizeof(buffer), &senderAdress, &senderAdressLength) == -1 || (MESSAGE_GET_TYPE(buffer) != MESSAGE_TYPE_ACK )){
                cout << "My child\n";
                system("gnome-terminal -x sh -c ./a.out");

            }
            else{
                cout << i << endl;
                i++;
            }
        }
        else{
            if(UDP_recieve(receiveSocket, &buffer, sizeof(buffer), &senderAdress, &senderAdressLength) == -1){ //egentlig bare timeout
                if (close(sendSocket) == -1){
                	perror("sendsock not shutdown");
                }
                if (close(receiveSocket) == -1){
                	perror("receive not shutdown");
                }
                sendSocket = UDP_init_socket_send(&sendAddress, MASTER_PORT);
                receiveSocket = UDP_init_socket_recieve(SLAVE_PORT);
                if(sendSocket == -1 || receiveSocket == -1){
                    cout << "Failed graduation\n";
                    sleep(2);
                    return 0;
                }
                isMaster = 1;
            }
            else if(MESSAGE_GET_TYPE(buffer) == MESSAGE_TYPE_I){
                i = MESSAGE_GET_VAL(buffer);
                sendMessage = MESSAGE_MAKE(MESSAGE_TYPE_ACK,0);
                UDP_send(sendSocket, &sendMessage, &sendAddress);
            }


        }
    }
}

void init(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH]){
    elev_init();
    for(int floor=0;floor<N_FLOORS;++floor){
        for(int w=0;w<REQUEST_MATRIX_WIDTH;++w){
            requestMatrix[floor][w] = 0; //TODO: endre til å lese fra fil
        }
    }
}