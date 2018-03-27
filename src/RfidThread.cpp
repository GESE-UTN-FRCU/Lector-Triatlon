#include <RfidThread.h>

RfidThread::RfidThread(ArduinoDayGlobals* _globals) : BaseThread(_globals){
	this->funcionesRFID = new RFID();
	this->funcionesRFID->setGlobals(_globals);
}

// Pregunta si deberia correr el hilo.
bool RfidThread::shouldRun(long time){
	return Thread::shouldRun(time);
}

// Codigo en caso de correr.
void RfidThread::run(){

	if(this->funcionesRFID->chequearLectura()){
		// Aca tendria que guardar una lectura en memoria.
		tone(this->globals->PIN_BUZZER,500);
		Serial.println("d");
	}else{
		Serial.println("c");
	}
	/*
	static bool ledStatus = false;
	ledStatus = !ledStatus;
	digitalWrite(8, ledStatus);

	Serial.println("El thread de RFID esta corriendo.");
	*/

	this->runned();
}
