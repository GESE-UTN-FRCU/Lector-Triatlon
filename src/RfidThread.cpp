#include <RfidThread.h>

RfidThread::RfidThread(ArduinoDayGlobals* _globals) : BaseThread(_globals){

}

bool RfidThread::chequearLectura(){
	// Chequea si hay una tarjeta para leer:
	if (!this->globals->rfid->PICC_IsNewCardPresent()||!this->globals->rfid->PICC_ReadCardSerial()) return false;
	// Chequea el tipo de tarjeta:
	MFRC522::PICC_Type piccType;
	piccType = this->globals->rfid->PICC_GetType(this->globals->rfid->uid.sak);
	if (piccType != MFRC522::PICC_TYPE_MIFARE_1K) {
		Serial.println("Tipo de tarjeta invalido.");
		return false;
	}
	return true;
}

// Pregunta si deberia correr el hilo.
bool RfidThread::shouldRun(long time){
	return Thread::shouldRun(time);
}

// Codigo en caso de correr.
void RfidThread::run(){
	if(this->RFID.chequearLectura()){
		// Aca tendria que guardar una lectura en memoria.
		digitalWrite(this->globals->PIN_BUZZER, HIGH);
		delay(500);
		digitalWrite(this->globals->PIN_BUZZER, LOW);
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
