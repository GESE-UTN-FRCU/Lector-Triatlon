#include <RFID.h>

bool RFID::chequearLectura(){
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

bool RFID::codigo_igual(byte codigo1[],byte codigo2[]){
  for (int i=0;i<=8;i++) if (codigo1[i]!=codigo2[i]) return false;
  return true;
}
