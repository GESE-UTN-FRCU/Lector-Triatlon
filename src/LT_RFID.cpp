#include <LT_RFID.h>

static void LT_RFID::iniciarModulo(){
	Globals::rfid->PCD_Init();
	return true;
}

static bool LT_RFID::chequearLectura(){
	// Chequea si hay una tarjeta para leer:
	if (!Globals::rfid->PICC_IsNewCardPresent()||!Globals::rfid->PICC_ReadCardSerial()) return false;
	// Chequea el tipo de tarjeta:
	MFRC522::PICC_Type piccType;
	piccType = Globals::rfid->PICC_GetType(Globals::rfid->uid.sak);
	if (piccType != MFRC522::PICC_TYPE_MIFARE_1K) {
		Serial.println("Tipo de tarjeta invalido.");
		return false;
	}
	return true;
}

static bool LT_RFID::codigo_igual(byte codigo1[],byte codigo2[]){
  for (int i=0;i<=8;i++) if (codigo1[i]!=codigo2[i]) return false;
  return true;
}