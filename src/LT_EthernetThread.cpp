#include <LT_EthernetThread.h>

LT_EthernetThread::LT_EthernetThread() : LT_BaseThread(){}

// Pregunta si deberia correr el hilo.
bool LT_EthernetThread::shouldRun(long time){
	return Thread::shouldRun(time);
}

// Codigo en caso de correr.
void LT_EthernetThread::run(){
	LT_Ethernet::procesarPaquetes();
	this->runned();
}