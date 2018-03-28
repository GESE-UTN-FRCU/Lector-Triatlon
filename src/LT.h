#ifndef _LT_H_
#define _LT_H_

// Librerias principales:
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <LT_Globals.h>

// Librerias de hilos:
#include <Thread.h>
#include <ThreadController.h>
#include <LT_BaseThread.h>

// Librerias de instancias de hilos:
#include <LT_SendThread.h>
#include <LT_ReceiveThread.h>
#include <LT_RfidThread.h>

class LT {
	private:
		
		static void initHardware();
		static void initThreadController();
		static void initPins();
		static void initEthernet();
		static void initRFID();
		static void initClock();
		static void initLCD();
	public:
		static ThreadController *threadController;
		static LT_SendThread *sendThread;
		static LT_ReceiveThread *receiveThread;
		static LT_RfidThread *rfidThread;
		
		static void setup();
		static void loop();
		static void reiniciarSistema();
};

#endif