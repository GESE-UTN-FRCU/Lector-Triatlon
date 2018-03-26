#ifndef _ARDUINO_DAY_CORE_H_
#define _ARDUINO_DAY_CORE_H_

// Librerias principales:
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include <ArduinoDayObject.h>

// Librerias de hilos:
#include <Thread.h>
#include <ThreadController.h>

// Librerias de hilos:
#include <BaseThread.h>
#include <SendThread.h>
#include <ReceiveThread.h>
#include <RfidThread.h>

class ArduinoDayCore : public ArduinoDayObject {
	private:
		ThreadController threadController;
		SendThread *sendThread;
		ReceiveThread *receiveThread;
		RfidThread *rfidThread;
		
		void initHardware();
		void initThreadController();
		void initPins();
		void initEthernet();
		void initRFID();
		void initClock();
		void initLCD();
	public:
		void setup();
		void loop();
		static void reiniciarSistema();
};

#endif