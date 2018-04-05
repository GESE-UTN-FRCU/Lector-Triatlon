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
		
		Serial.println("Detectando tarjeta...");

		LT_MemoriaReloj::escribirLecturaMemoria(millis(),Globals::ultimaLectura);

		Serial.println(LT_MemoriaReloj::leerUltimoCodigo());
		Serial.println(LT_MemoriaReloj::leerUltimoTiempo());
		Serial.println(Globals::ultimaLectura);
		
		
		
		
		


	}

	this->runned();
}
