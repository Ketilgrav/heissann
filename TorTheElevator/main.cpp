#include <thread>
#include <mutex>
#include <time.h>

using namespace std;

typedef message;

struct Button {
	int floor;
	int button;
	Button(int floor, int button) {
		set(floor, button);
	}
	Button() {
		deactivate();
	}
	void deactivate() {
		floor = -1;
	}
	bool isActive() {
		floor = 1;
	}
	void set(int floor, int button) {
		this->floor = floor;
		this->button = button;
	}
};


#define NULL_MESSAGE
#define NULL_BUTTON Button(-1,-1);

#define NUM_FLOORS
#define TIME_OUT

#define MESSAGE_TYPE 0
#define MESSAGE_BUTTON_FLOOR 1 
#define MESSAGE_BUTTON_TYPE 2 
#define MESSAGE_COST 3
#define MESSAGE_TIMEOUT 3

int main(){
	bool requestMatrix[NUM_FLOORS][4];
	time_t timeOut[NUM_FLOORS];
	time_t currentTime;

	int finishedRequests = NULL;
	mutex finishedRequestsMutex;
	thread stateMachine(state_machine, &requestMatrix, &finishedRequests,&finishedRequestsMutex);

	int[3] recieved = NULL_MESSAGE;
	mutex recievedMutex;
	thread recieve(UDP_recieve,&recieve,&recievedMutex);

	Button buttonPress;


	time_t newRequestsTimeInn[NUM_FLOORS];
	bool newReqestResponsability[NUM_FLOORS];

	unsigned cost = INT16_MAX;

	while(1){
		buttonPress = get_button_press();

		if(buttonPress != NULL_BUTTON){
			cost = calculateCost(requestMatrix);
			requestMatrix[buttonPress.floor][buttonPress.button] = 1;
			requestMatrix[buttonPress.floor][TIME_OUT] = 0; //TODO: Sett tiden
			//Transmit knappen med cost
			newRequestsTimeInn[buttonPress.floor] = 0;//TODO:  om 2sec ellerno
			newRequestOur[floor]= true;
			buttonPress.deactivate();
		}

		if (recieved != NULL_MESSAGE) {
			if (recieved.type == REQUEST) {
				requestMatrix[recieved[MESSAGE_BUTTON_FLOOR][buttonPress.button] = 1;
				requestMatrix[buttonPress.floor][TIME_OUT] = 0; //TODO: Sett tiden //ENDRE TIL MESSAGE
				cost = calculateCost;
				if(cost < ) //Om vår kost er strengt bedre
					//Transmitt knappen med vår cost 
					//newRequestsTimeInn[floor] = om 2secish
					//newReqestOur[floor] = 1;
				//er vi strengt dårligere:
					//newReqestOur[floor] = 0; //den trer ikke i kraft lengre
			}
			else if (recieved.type == PROCESSED) {
				//Sjekk at meldingen ikke har timet ut
				//Hvis ikke: fjern opp og ned fra den etasjen. Og at jeg skal gjøre den om ikke opperatør knappen er trykket? (os sette timeout til inf)
			}
		}

		//GÅ gjennom newRquestsTimeIn, og se om det skal skje noe
			//Sett tiden til inf
			//Om newRequestOur == 1
				//Skriv at vi skal løse problemet i matrisen
				//newRequestOur[2] = 0;
				

		if (finishedRequests != NULL) {
			//Send meldingen at denne er ordnet, legg ved timeout før dørene lukkes
			//Tøm den etasjen (hele, uansett). (innebærer å sette timeout til inf)
		}

		//Sjekke om noen har gått ut på dato
		for (int floor = 0; floor < NUM_FLOORS; ++floor) {
			currentTime = time(NULL);
			if (timeOut[floor] != 0 && timeOut[floor] > currentTime) {
				//Retransmit etasjen som en ny request
			}
		}
	}

}

int calculateCost(bool** requestMatrix) {

}