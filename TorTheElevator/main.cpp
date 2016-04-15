#include <thread>
#include <atomic>

#include <fstream>
#include <iostream>
#include <string>

#include <sys/time.h>

#include "main_include.h"
#include "state_machine.h"
#include "event_manager.h"

using namespace std;

#define SLEEPTIME_uS 50*1000
#define TIMEOUT 3*SLEEPTIME_uS


void init(bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], fstream* fMaster){
    fMaster->seekg(0);
    uint64_t t;
    char chr;
    (*fMaster) >> t;
    fMaster->get(chr);  //Fjerner '\n' etter første linje.
    cout << "Reading time: " << endl;
    cout << "time: " << t << endl;
    elev_init();
    for(int floor=0;floor<N_FLOORS;++floor){
        for(int w=0;w<REQUEST_MATRIX_WIDTH;++w){
            if(fMaster->get(chr)){
                cout << ":" << chr << ":";
                requestMatrix[floor][w] = chr - '0';
            }
            else{
                requestMatrix[floor][w] = 0;
            }
            
        }
        cout << endl;
    }
    cout << "Initialization completed." << endl;
}

int main(){
    bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH];

    /*****
    Slave process pair
    *****/

    fstream fMaster;

    /*Opens the process pair communication files for reading.
    If the files does not exist then it has to be made.*/
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
        cout << "Failed to open files." << endl;
        sleep(1);
        exit(1);
    }


    timeval tv;
    uint64_t t = 0;
    uint64_t tNow = 0;
    while(!isMaster){
    	gettimeofday(&tv,0);
    	tNow = tv.tv_sec*1000000+tv.tv_usec;

        fSlave.seekg(0);
        fSlave << tNow << endl;

        usleep(SLEEPTIME_uS);

        fMaster.seekg(0);
        fMaster >> t;
        if(t+TIMEOUT<tNow){
            cout << "Master timed out, all your base are belong to slave." << endl;
            //system("gnome-terminal -x sh -c ./executable");
            isMaster = 1;
        }
    }



    /*****
    Master process pair
    *****/
    init(requestMatrix, &fMaster);
        
    atomic<int> finishedFloor(-1);
    atomic<int> latestFloor;
    thread state_machine_thread(state_machine, requestMatrix, &finishedFloor, &latestFloor);
    thread event_manager_thread(event_manager, requestMatrix, &finishedFloor, &latestFloor);

    
    fMaster.close();
    fSlave.close();
    fMaster.open("requestMatrix.txt", fstream::out);
    fSlave.open("slavePing.txt", fstream::in);
    if(!fSlave.is_open() || !fMaster.is_open()){
        cout << "Failed to open" << endl;
        usleep(1);
        exit(1);
    }


    while(1){
       	gettimeofday(&tv,0);
		tNow = tv.tv_sec*1000000+tv.tv_usec;

        fMaster.seekg(0);
        fMaster << tNow << endl;
        for(int floor=0;floor<N_FLOORS;++floor){
            for(int v=0;v<REQUEST_MATRIX_WIDTH;++v){
                fMaster << requestMatrix[floor][v];
            }
        }
        fMaster << endl;
        
        usleep(SLEEPTIME_uS);

        /*fSlave.seekg(0);
        fSlave >> t;
        if(t+TIMEOUT<tNow){
            cout << "Slave timed out, making new." << endl;
            system("gnome-terminal -x sh -c ./executable");
        }*/
    }

    return 0;



}

