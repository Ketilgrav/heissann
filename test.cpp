#include <time.h>
#include <iostream>

using namespace std;

int main(){
	int a=5;
	int b=1235;
	int c=23;
	int d=56312;
	uint8_t checkSum = 255-(a+b+c+d)%255;
	cout << checkSum << endl;
	cout << (a+b+c+d+checkSum)%255 << endl;
}
