#include <iostream>
#include <pthread.h>
using namespace std;


int i = 0;



void* foo1(void* arg){
    for(int j = 0; j<1000001; ++j){

        i++;

    }
    return NULL;
}

void* foo2(void* x){
    for(int j = 0; j<1000000; ++j){
        i--;

    }
    return NULL;
}


int main() {
    pthread_t first;
    pthread_t second;


    pthread_create(&first, NULL, foo1, NULL);
    pthread_create(&second, NULL, foo2, NULL);


    pthread_join(first,NULL);
    pthread_join(second,NULL);

    cout << "Donde esta\n";
    cout << i << endl;
    return 0;
}