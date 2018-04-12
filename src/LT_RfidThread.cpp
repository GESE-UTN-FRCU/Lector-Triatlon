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

		uint32_t milisegundos = millis();
		
		Serial.println("Detectando tarjeta.");

		LT_MemoriaReloj::escribirLecturaMemoria(milisegundos,Globals::ultimaLectura);

		// Serial.println(LT_MemoriaReloj::leerUltimoCodigo());
		// Serial.println(LT_MemoriaReloj::leerUltimoTiempo());

		//LT_Ethernet::enviarInfo(String(milisegundos),String(Globals::ultimaLectura));
		LT_Ethernet::enviarInfo(milisegundos,Globals::ultimaLectura);
		
		LT_LCD::cambiarLineaLCD("Leido");

		tone(Globals::PIN_BUZZER,880,500);
	}

	this->runned();
}
