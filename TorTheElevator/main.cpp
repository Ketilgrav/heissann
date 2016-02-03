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
			//Skriv inn knappen, og timeout, men ikke hvem som skal gj�re den.
			//Transmit knappen med cost
			//newRequestsTimeInn[floor] = om 2sec ellerno
			//newRequestOur[floor]= true;
		}

		if (recieved != NULL_MESSAGE) {
			if (recieved.type == REQUEST) {
				//Skriv inn requesten, og timeout, men ikke hvem som skal gj�re den. 
				//Beregn costen v�r for denne oppgaven.
				//Er vi strengt bedre?
					//Transmitt knappen med v�r cost 
					//newRequestsTimeInn[floor] = om 2secish
					//newReqestOur[floor] = 1;
				//er vi strengt d�rligere:
					//newReqestOur[floor] = 0; //den trer ikke i kraft lengre
			}
			else if (recieved.type == PROCESSED) {
				//Sjekk at meldingen ikke har timet ut
				//Hvis ikke: fjern opp og ned fra den etasjen. Og at jeg skal gj�re den om ikke opperat�r knappen er trykket? (os sette timeout til inf)
			}
		}

		//G� gjennom newRquestsTimeIn, og se om det skal skje noe
			//Sett tiden til inf
			//Om newRequestOur == 1
				//Skriv at vi skal l�se problemet i matrisen
				//newRequestOur[2] = 0;
				

		if (finishedRequests != NULL) {
			//Send meldingen at denne er ordnet, legg ved timeout f�r d�rene lukkes
			//T�m den etasjen (hele, uansett). (inneb�rer � sette timeout til inf)
		}

		//Sjekke om noen har g�tt ut p� dato
		for (int floor = 0; floor < NUM_FLOORS; ++floor) {
			currentTime = time(NULL);
			if (timeOut[floor] != 0 && timeOut[floor] > currentTime) {
				//Retransmit etasjen som en ny request
			}
		}
	}

}