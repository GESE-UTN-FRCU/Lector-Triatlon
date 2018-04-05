#ifndef _LT_H_
#define _LT_H_

// Librerias principales:
#include <Arduino.h>
#include <LT_Globals.h>
#include <SPI.h>
#include <Wire.h>


// Librerias de modulos:
#include <LT_RFID.h>
#include <LT_Ethernet.h>
#include <LT_LCD.h>
#include <LT_MemoriaReloj.h>

// Librerias de hilos:
#include <Thread.h>
#include <ThreadController.h>
#include <LT_BaseThread.h>

// Librerias de instancias de hilos:
#include <LT_RfidThread.h>
#include <LT_EthernetThread.h>
#include <LT_ReceiveThread.h>
#include <LT_SendThread.h>

class LT {
	private:
		static ThreadController *threadController;
		static LT_RfidThread *rfidThread;
		static LT_EthernetThread *ethernetThread;
		static LT_ReceiveThread *receiveThread;
		static LT_SendThread *sendThread;
		
		static void initHardware();
		static void initThreadController();
		static void initPins();
		static void initEthernet();
		static void initRFID();
		static void initClock();
		static void initLCD();
		
	public:
		static void setup();
		static void loop();
		static void reiniciarSistema();
};

#endif
