#include "network.h"
#include <iostream>

using namespace std;


#define RECEIVE_PORT 20013
#define SEND_PORT 20013
const char BROADCAST_IP[16] = "129.241.187.255";


enum MessageType{
	messageRequest = 0,
	messageComplete = 1
};

struct Data{
	MessageType msgType;
	uint8_t floor;
	uint8_t button;
	uint16_t price;
	Data(MessageType msgType, uint8_t floor, uint8_t button, uint16_t price){
		this->msgType = msgType;
		this->floor = floor;
		this->button = button;
		this->price = price;
	}
	Data();
};


int main(){
	MessageType testmessage = messageRequest;
	Data testdata(testmessage, 2, 1, 3011);

    NetworkMessage network(RECEIVE_PORT,SEND_PORT, BROADCAST_IP, 2);

    network.send_message(&testdata);
    Data recData;
    while(!network.receive_message(&recData));
    testdata.floor =3;
    cout << recData.floor << endl;

}