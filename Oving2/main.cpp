#include <iostream>
#include <pthread.h>
using namespace std;


int i = 0;
pthread_mutex_t mutexVar;


void* foo1(void* arg){
    for(int j = 0; j<1000001; ++j){
        pthread_mutex_lock(&mutexVar);
        i++;
        pthread_mutex_unlock(&mutexVar);
    }
    return NULL;
}

void* foo2(void* x){
    for(int j = 0; j<1000000; ++j){
        pthread_mutex_lock(&mutexVar);
        i--;
        pthread_mutex_unlock(&mutexVar);
    }
    return NULL;
}


int main() {
    pthread_mutex_init(&mutexVar, NULL);

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