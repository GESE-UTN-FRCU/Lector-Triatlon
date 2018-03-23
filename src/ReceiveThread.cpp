#include "ReceiveThread.h"

// Pregunta si deberia correr el hilo.
bool ReceiveThread::shouldRun(long time){
	return Thread::shouldRun(time);
}

// Codigo en caso de correr.
void ReceiveThread::run(){
	static bool ledStatus = false;
	ledStatus = !ledStatus;
	digitalWrite(5, ledStatus);

	Serial.println("a");

	this->runned();
}
