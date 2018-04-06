#include <LT_RfidThread.h>

LT_RfidThread::LT_RfidThread() : LT_BaseThread(){}

// Pregunta si deberia correr el hilo.
bool LT_RfidThread::shouldRun(long time){
	return Thread::shouldRun(time);
}

// Codigo en caso de correr.
void LT_RfidThread::run(){
	static StaticJsonBuffer<52> jsonBuffer;

	if(LT_RFID::nuevaLectura()){
		// Aca tendria que guardar una lectura en memoria.
		
		Serial.println("Detectando tarjeta...");

		//LT_MemoriaReloj::escribirLecturaMemoria(millis(),Globals::ultimaLectura);

		// Serial.println(LT_MemoriaReloj::leerUltimoCodigo());
		// Serial.println(LT_MemoriaReloj::leerUltimoTiempo());

		uint32_t milisegundos = millis();

		Serial.print("Lectura nueva con el codigo: ");
		Serial.print(Globals::ultimaLectura);
		Serial.print(" y los milisegundos: ");
		Serial.println(milisegundos);

		Serial.print("Memoria disponible: ");
		Serial.println(freeMemory());

		JsonObject& JSON_data = jsonBuffer.createObject();
		JSON_data["codigo"] = Globals::ultimaLectura;
		JSON_data["tiempo"] = milisegundos;
		//LT_Ethernet::enviarJSON("POST", "actions/lectura.js", JSON_data);
	}

	this->runned();
}
