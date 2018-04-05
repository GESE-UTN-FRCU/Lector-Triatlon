#include <LT_RFID.h>

static void LT_RFID::iniciarModulo(){
	Globals::rfid->PCD_Init();
	return true;
}

static bool LT_RFID::hayLectura(){
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

static bool LT_RFID::nuevaLectura(){

	if(hayLectura()){
		uint32_t codigo;
		memcpy((byte*) &codigo, &Globals::rfid->uid.uidByte, Globals::rfid->uid.size);
		Globals::rfid->PICC_HaltA();
		Globals::rfid->PCD_StopCrypto1();
		if(Globals::ultimaLectura!=codigo){
			memcpy(&Globals::ultimaLectura, &codigo, 4);
			return true;
		}
	}
	return false;
}

static bool LT_RFID::compararCodigos(byte a[], byte b[]){
	for (uint8_t i=0; i<=8; i++) if (a[i]!=b[i]) return false;
	return true;
}
