#ifndef _LT_GLOBALS_H_
#define _LT_GLOBALS_H_

// Librerias de componentes:
#include <EEPROM.h>
#include <MFRC522.h>
#include <EtherCard.h>
#include <LiquidCrystal_SR_LCD3.h>
#include <DS3231.h>
//#include <ArduinoJson.h>

#define ETH_BUFFER_SIZE 450

class LT_Globals {
    public:
		// Direcciones de memoria EEPROM del arduino
		static const uint8_t  ADDR_MODO_CONFIG = 0;
		static const uint8_t  ADDR_MYIP = 2;
		static const uint8_t  ADDR_GWIP = 6;
		static const uint8_t  ADDR_HISIP = 10;
		static const uint8_t  ADDR_NETMASK = 14;
		static const uint8_t  ADDR_HISPORT = 18;
		static const uint8_t  ADDR_INDICE = 19;	

		// Variables de la memoria del reloj 			
		static uint8_t indice;
		static const uint8_t tamano=4;

		//Variables del RFID
		static uint32_t ultimaLectura;

		// Lista de pines:
		static const int PIN_BOTON = 2;
		static const int PIN_LCD_STROBE = 3;
		static const int PIN_LCD_DATA = 4;
		static const int PIN_LCD_CLOCK = 5;
		static const int PIN_LCD_LED = 6;
		static const int PIN_MFRC522_RST = 7;
		static const int PIN_MFRC522_SDA = 8;
		static const int PIN_BUZZER = 9;
		static const int PIN_ETH_SDA = 10;

    	// Configuracion general:
		static const uint32_t SERIAL_FREQ = 57600;
		//static StaticJsonBuffer<56> jsonBuffer;

		// Configuracion de la placa ethernet:
		static uint8_t   mymac[6];
		static uint8_t    myip[4];
		static uint8_t    gwip[4];
		static uint8_t   hisip[4];
		static uint8_t   dnsip[4];
		static uint8_t netmask[4];
		static uint8_t hisport;
		static const uint16_t ETHERNET_BUFFER_SIZE = ETH_BUFFER_SIZE;

		// Caracteres LCD:
		static const uint8_t CHAR_UTN = 0;

		// Instancias de utilidades:
		//static uint8_t session;
		static word pos;
		static char postBuffer[48];
		

		// Instancias de dispositivos:
		static MFRC522 *rfid;
		static EtherCard *ethernet;
		static LiquidCrystal_SR_LCD3 *lcd;
		static DS3231 Clock;
};

typedef LT_Globals Globals;

#endif
