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

		//LT_MemoriaReloj::escribirLecturaMemoria(millis(),Globals::ultimaLectura);

		// Serial.println(LT_MemoriaReloj::leerUltimoCodigo());
		// Serial.println(LT_MemoriaReloj::leerUltimoTiempo());

		uint32_t milisegundos = millis();

		/*
		StaticJsonBuffer<128> jsonBuffer;
		JsonObject& JSON_data = jsonBuffer.createObject();
		JSON_data["codigo"] = Globals::ultimaLectura;
		JSON_data["tiempo"] = milisegundos;

		String output;
		JSON_data.printTo(output);
		Serial.println(output);

		*/
		//LT_Ethernet::enviarJSON("POST", "actions/lectura.js", JSON_data);
		
		LT_Ethernet::enviarAlgo();

		tone(Globals::PIN_BUZZER,880,500);
	}

	this->runned();
}
