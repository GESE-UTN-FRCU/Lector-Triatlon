#include <LT_SendThread.h>

LT_SendThread::LT_SendThread() : LT_BaseThread(){}

// Pregunta si deberia correr el hilo.
bool LT_SendThread::shouldRun(long time){
	return Thread::shouldRun(time);
}

// Codigo en caso de correr.
void LT_SendThread::run(){
	static uint16_t counter = 0;
	Serial.print("SendThread: ");
	Serial.println(counter++);
	this->runned();
}