#include <fstream>
#include <string>
#include <iostream>
#include <time.h>
#include <unistd.h>

using namespace std;

#define N_FLOORS 4
#define REQUEST_MATRIX_WIDTH 4
#define TIMEOUT 3

void init(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], fstream* fMaster){
    fMaster->seekg(0);
    int t;
    char chr;
    (*fMaster) >> t;
    fMaster->get(chr);
    //elev_init();
    for(int floor=0;floor<N_FLOORS;++floor){
        for(int w=0;w<REQUEST_MATRIX_WIDTH;++w){
            if(fMaster->get(chr)){
                cout << ":" << chr<< ":" <<endl;
                requestMatrix[floor][w] = chr - '0';
            }
            else{
                requestMatrix[floor][w] = 0;
            }
            
        }
    }
}

int main(){
	bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH];
    fstream fMaster;
    fMaster.open("requestMatrix.txt", fstream::in);
    if(!fMaster.is_open()){
        fMaster.close();
        fMaster.open("requestMatrix.txt", fstream::out);
        fMaster.close();
        fMaster.open("requestMatrix.txt", fstream::in);
    }
    fstream fSlave("slavePing.txt",fstream::out);


    bool isMaster = 0;
    if(!fSlave.is_open() || !fMaster.is_open()){
        cout << "Failed to open" << endl;
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

    init(requestMatrix, &fMaster);
    fMaster.close();
    fSlave.close();

    atomic<int> finishedFloor(-1);
    atomic<int> latestFloor;
    thread state_machine_thread(state_machine, requestMatrix, &finishedFloor, &latestFloor);

    

    fMaster.open("requestMatrix.txt", fstream::out);
    fSlave.open("slavePing.txt", fstream::in);
    if(!fSlave.is_open() || !fMaster.is_open()){
        cout << "Failed to open" << endl;
        sleep(1);
        exit(1);
    }

    while(1){
        fMaster.seekg(0);
        fMaster << time(NULL) << endl;
        for(int floor=0;floor<N_FLOORS;++floor){
            for(int v=0;v<REQUEST_MATRIX_WIDTH;++v){
                fMaster << requestMatrix[floor][v];
            }
        }
        fMaster << endl;
        
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

