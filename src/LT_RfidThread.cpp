#include <LT_RfidThread.h>

#define PIN_BUZZER 		9

LT_RfidThread::LT_RfidThread() : LT_BaseThread(){}

// Pregunta si deberia correr el hilo.
bool LT_RfidThread::shouldRun(long time){
	return Thread::shouldRun(time);
}

// Codigo en caso de correr.
void LT_RfidThread::run(){

	if(LT_RFID::nuevaLectura()){

		uint32_t milisegundos = millis();
		
		Serial.println(F("Detectando tarjeta"));

		// Aca tendria que guardar una lectura en memoria (no funciona bien).
		LT_MemoriaReloj::escribirLecturaMemoria(milisegundos,Globals::ultimaLectura);
		// Serial.println(LT_MemoriaReloj::leerUltimoCodigo());
		// Serial.println(LT_MemoriaReloj::leerUltimoTiempo());


		// Aca se manda la lectura.
		//LT_Ethernet::enviarLectura(milisegundos,Globals::ultimaLectura);
		
		LT_LCD::cambiarLineaLCD("Leido");

		tone(PIN_BUZZER,880,500);
	}

	this->runned();
}
