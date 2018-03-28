#include <LT_RfidThread.h>

LT_RfidThread::LT_RfidThread() : LT_BaseThread(){}

// Pregunta si deberia correr el hilo.
bool LT_RfidThread::shouldRun(long time){
	return Thread::shouldRun(time);
}

// Codigo en caso de correr.
void LT_RfidThread::run(){

	if(LT_RFID::nuevaLectura()){
		// Aca tendria que guardar una lectura en memoria.
		tone(Globals::PIN_BUZZER,500);
		Serial.println("Detectando tarjeta...");
	}

	static uint16_t counter = 0;
	Serial.print("SendThread: ");
	Serial.println(counter++);

	this->runned();
}