#ifndef _ARDUINO_GLOBALS_H_
#define _ARDUINO_GLOBALS_H_

// Librerias de componentes:
#include <EEPROM.h>
#include <MFRC522.h>
#include <EtherCard.h>
#include <LiquidCrystal_SR_LCD3.h>
#include <DS3231.h>
#include <ArduinoJson.h>

class ArduinoDayGlobals {
	public:
		// Lista de pines:
		const uint8_t PIN_MODO_CONFIG  = 2;
		const uint8_t PIN_LCD_STROBE   = 3;
		const uint8_t PIN_LCD_DATA     = 4;
		const uint8_t PIN_LCD_CLOCK    = 5;
		const uint8_t PIN_MFRC522_RST  = 6;
		const uint8_t PIN_MFRC522_SDA  = 8;
		const uint8_t PIN_BUZZER       = 9;
		const uint8_t PIN_ETH_SDA      = 10;

		// Configuracion de la placa ethernet:
		uint8_t   mymac[6]; //{ 0x74,0x69,0x69,0xAA,0x30,0x20 }
		uint8_t    myip[4]; //{ 192,168,8,152 }
		uint8_t    gwip[4]; //{ 192,168,8,1 };
		uint8_t   hisip[4]; //{ 192,168,8,222 };
		uint8_t   dnsip[4]; //{ 8,8,8,8 };
		uint8_t netmask[4]; //{ 255,255,255,0 };
		uint8_t hisport=80;
		uint8_t session;
		Stash stash;

		// Instancias de dispositivos:
		StaticJsonBuffer<200> jsonBuffer;
		MFRC522 *rfid;
		EtherCard *ethernet;
		LiquidCrystal_SR_LCD3 *lcd;
};

#endif