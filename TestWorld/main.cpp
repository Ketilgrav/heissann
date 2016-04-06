#include <thread>
#include <atomic>
#include <iostream>
#include <unistd.h>
using namespace std;

void add(atomic<int>* a){
	for(unsigned i=0;i<100001;++i){
		(*a)++;
	}
}

void sub(atomic<int>* a){
	for(unsigned i=0;i<100000;++i){
		(*a)--;
	}
}


void st(int **a, atomic<int>* b){
	while(1){
		a[1][1] = (*b);
	}
}


enum ketil{
	stupid = -1,
	moron = 0,
	ok = 1
};

int main(){
	ketil guy = ok;
	guy= ketil(-guy);
	cout << guy;	

/*
	int a[3][3];
	a[1][1] = 4;
	atomic<int> b(2);
	thread sthread(st, a, &b);

	b=6;

	b=2;
	b=1;
	
	usleep(3);
	cout << a[1][1];
	sthread.detach();
	cout << "START\n";
	atomic<int> a;
	a=1;
	thread addThrd(add, &a);
	thread subThrd(sub, &a);

	int h = a;
	addThrd.join();
	subThrd.join();
	cout << a << endl;
	cout << "END\n";*/
	return 0;
}