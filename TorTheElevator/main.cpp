#include <thread>
#include <mutex>
#include <time.h>

using namespace std;

typedef message;
typedef button;

#define NULL_MESSAGE
#define NULL_BUTTON

#define NUM_FLOORS

int main(){
	bool requestMatrix[NUM_FLOORS][4];
	time_t timeOut[NUM_FLOORS];
	time_t currentTime;

	int finishedRequests = NULL;
	mutex finishedRequests_mutex;
	thread stateMachine(state_machine, &requestMatrix, &finishedRequests,&finishedRequests_mutex);

	message recieved = NULL_MESSAGE;
	mutex recieved_mutex;
	thread recieve(UDP_recieve,&recieve,&recieved_mutex);

	button buttonPress = NULL_BUTTON;


	time_t newRequestsTimeInn[NUM_FLOORS];
	bool newReqestOur[NUM_FLOORS];

	while(1){
		buttonPress = get_button_press();

		if(buttonPress != NULL_BUTTON){
			//Beregn cost
			//Skriv inn knappen, og timeout, men ikke hvem som skal gjøre den.
			//Transmit knappen med cost
			//newRequestsTimeInn[floor] = om 2sec ellerno
			//newRequestOur[floor]= true;
		}

		if (recieved != NULL_MESSAGE) {
			if (recieved.type == REQUEST) {
				//Skriv inn requesten, og timeout, men ikke hvem som skal gjøre den. 
				//Beregn costen vår for denne oppgaven.
				//Er vi strengt bedre?
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