#include <LT_ReceiveThread.h>

LT_ReceiveThread::LT_ReceiveThread() : LT_BaseThread(){}

// Pregunta si deberia correr el hilo.
bool LT_ReceiveThread::shouldRun(long time){
	return Thread::shouldRun(time);
}

// Codigo en caso de correr.
void LT_ReceiveThread::run(){
	static uint16_t counter = 0;
	Serial.print("ReceiveThread: ");
	Serial.println(counter++);
	
	this->runned();
}