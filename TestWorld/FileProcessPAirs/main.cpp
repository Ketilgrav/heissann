#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h>
#include <time.h>

using namespace std;

#define TIMEOUT 3

int main(){
	fstream fMaster("requestMatrix.txt", fstream::out);
	fMaster.close();
	fMaster.open("requestMatrix.txt", fstream::in);
	if(!fMaster.is_open()){
		cout << "Failed to open master" << endl;
		sleep(1);
		exit(1);
	}

	fstream fSlave("slavePing.txt",fstream::out);
	bool isMaster = 0;
	if(!fSlave.is_open()){
		cout << "Failed to open slave" << endl;
		sleep(1);
		exit(1);
	}


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
		cout << "robin" << endl;
	}

	fMaster.close();
	fSlave.close();

	fMaster.open("requestMatrix.txt", fstream::out);
	if(!fMaster.is_open()){
		cout << "Failed to open master" << endl;
		sleep(1);
		exit(1);
	}

	fSlave.open("slavePing.txt",fstream::in);
	if(!fSlave.is_open()){
		cout << "Failed to open slave" << endl;
		sleep(1);
		exit(1);
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
		cout << "batman" << endl;
	}

	return 0;
}