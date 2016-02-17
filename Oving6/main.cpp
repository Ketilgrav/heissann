#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <time.h>

#include "Network.h"

using namespace std;


//MESSAGE_TYPE
#define MESSAGE_NOMSG 0
#define MESSAGE_TYPE_ACK 1
#define MESSAGE_TYPE_MYADDR 2
#define MESSAGE_TYPE_I 3
#define MESSAGE_TYPE_MYSLAVEIS 4

#define MESSAGE_TYPE_LOCATION 29
#define MESSAGE_MASK 0x1fffffff

#define MESSAGE_MAKE(type,val) ((type << MESSAGE_TYPE_LOCATION)|(val&MESSAGE_MASK))
#define MESSAGE_GET_TYPE(msg) (msg>>MESSAGE_TYPE_LOCATION)
#define MESSAGE_GET_VAL(msg) (msg&MESSAGE_MASK)

int main() {
    sockaddr_in sendAddress;
    sockaddr_in senderAdress;
    socklen_t senderAdressLength;
    uint32_t buffer;
    int sendSocket = UDP_init_socket_send(&sendAddress, SLAVE_PORT);
    int receiveSocket = UDP_init_socket_recieve(MASTER_PORT);

    if(sendSocket == -1 || receiveSocket == -1){
        cout << "Failed to connect to socket\n";
        return 0;
    }


    int i = 0;
    bool isMaster = 0;

    uint32_t sendMessage;

    while(1){
        if(isMaster) {
            printf("i am master\n");
            sendMessage = MESSAGE_MAKE(MESSAGE_TYPE_I,i);
            UDP_send(sendSocket, &sendMessage, &sendAddress);

            buffer = 0;

            if(UDP_recieve(receiveSocket, &buffer, 32, &senderAdress, &senderAdressLength) == -1 || (MESSAGE_GET_TYPE(buffer) != MESSAGE_TYPE_ACK )){
                cout << "My child\n";
                system("./a.out");

            }
            else{
                cout << i << endl;
                i++;
            }
        }
        else{
            if(UDP_recieve(receiveSocket, &buffer, 32, &senderAdress, &senderAdressLength) == -1){ //egentlig bare timeout
                cout << "I am your master, Luke\n";
                shutdown(sendSocket, SHUT_WR);
                shutdown(receiveSocket, SHUT_RD);
                sendSocket = UDP_init_socket_send(&sendAddress, MASTER_PORT);
                receiveSocket = UDP_init_socket_recieve(SLAVE_PORT);
                if(sendSocket == -1 || receiveSocket == -1){
                    cout << "Failed graduation\n";
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