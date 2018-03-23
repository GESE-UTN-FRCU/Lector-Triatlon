#include "RfidThread.h"
#include <Thread.h>

RfidThread::RfidThread(): Thread(){
	/*
	pin = _pin;
	duration = _duration;
	_lastButtonPushed = 0;


	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
	*/

	enabled = true;
}

//Aca iria codigo personalizado si deberia correr.
bool shouldRun(long time){
	return true;
	//return Thread::shouldRun();
}

//Aca iria codigo personalizado si tiene que correr
void run(){
	/*
	static bool ledStatus = false;
	ledStatus = !ledStatus;

	digitalWrite(pin, ledStatus);

	//Aca iria codigo personalizado cuando corre
	*/
	Thread::run();
}
