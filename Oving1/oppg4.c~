#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdint.h>

using namespace std;

#define NUM_THREADS     5

int i = 0;

void *Increment(){
	for(uint32_t j = 0; j< 1000000; ++j){
		i++;
	}
	pthread_exit(NULL);
}

void *Decrement(){
	for(uint32_t j = 0; j< 1000000; ++j){
		i--;
	}
	pthread_exit(NULL);
}

int main ()
{
	pthread_t thread1, thread2;
	if(pthread_create(&thread1, NULL, Increment){
		cout << "ERROR";
		return(0);
	}
	if(pthread_create(&thread2, NULL, Decrement){
		cout << "ERROR";
		return(0);
	}
	pthread_exit(NULL);
e	return(0);
}
