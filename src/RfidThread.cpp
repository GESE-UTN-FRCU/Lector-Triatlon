#include "RfidThread.h"

//Constructor del thread. Set de variables.
RfidThread::RfidThread(){

	enabled = true;
}

//Pregunta si deberia correr el hilo
bool RfidThread::shouldRun(long time){
	return Thread::shouldRun();
}

//Codigo en caso de correr
void RfidThread::run(){

	static bool ledStatus = false;
	ledStatus = !ledStatus;
	digitalWrite(13, ledStatus);

	Serial.println("El thread de RFID esta corriendo.");

	runned();
}
