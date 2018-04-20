#include <LT_RFID.h>

#define PIN_MFRC522_RST 7
#define PIN_MFRC522_SDA 8

// Variables de uso de la placa RFID.
MFRC522 rfid(PIN_MFRC522_SDA, PIN_MFRC522_RST);

static void LT_RFID::iniciarModulo(){

	pinMode(PIN_MFRC522_SDA, OUTPUT);
	digitalWrite(PIN_MFRC522_SDA, HIGH);

	rfid.PCD_Init();
}

static bool LT_RFID::hayLectura(){
	// Chequea si hay una tarjeta para leer:
	if (!rfid.PICC_IsNewCardPresent()||!rfid.PICC_ReadCardSerial()) return false;
	// Chequea el tipo de tarjeta:
	MFRC522::PICC_Type piccType;
	piccType = rfid.PICC_GetType(rfid.uid.sak);
	if (piccType != MFRC522::PICC_TYPE_MIFARE_1K) {
		Serial.println("Tipo de tarjeta invalido.");
		return false;
	}
	return true;
}

static bool LT_RFID::nuevaLectura(){

	if(hayLectura()){
		uint32_t codigo;
		memcpy((byte*) &codigo, &rfid.uid.uidByte, rfid.uid.size);
		rfid.PICC_HaltA();
		rfid.PCD_StopCrypto1();
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
