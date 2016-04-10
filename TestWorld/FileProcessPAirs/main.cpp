#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;

#define TIMEOUT 3

int main(){
	fstream fMaster("requestMatrix.txt",fstream::out);
	fstream fSlave("slavePing.txt",fstream::out);
	bool isMaster = 0;

	time_t t = 0;
	while(!isMaster){
		fSlave.seekg(0);
		fSlave << time(NULL) << endl;

		sleep(1);

		fMaster.seekg(0);
		fMaster >> t;
		if(t+TIMEOUT<time(NULL)){
			system("gnome-terminal -x sh -c ./executable");
			isMaster = 1;
		}
		cout << "robin\n";
	}

	while(1){
		fMaster.seekg(0);
		fMaster << time(NULL) << endl;
		
		sleep(1);

		fSlave.seekg(0);
		fSlave >> t;
		if(t+TIMEOUT<time(NULL)){
			system("gnome-terminal -x sh -c ./executable");
		}
		cout << "batman\n";
	}

	return 0;
}