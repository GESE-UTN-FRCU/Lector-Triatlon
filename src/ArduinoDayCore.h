#ifndef _ARDUINO_DAY_H_
#define _ARDUINO_DAY_H_

// Librerias principales:
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <ArduinoDayGlobals.h>

// Librerias de hilos:
#include <Thread.h>
#include <ThreadController.h>
#include <BaseThread.h>
#include <SendThread.h>
#include <ReceiveThread.h>
#include <RfidThread.h>

//uint8_t Ethernet::buffer[514];

class ArduinoDayCore {
	private:
		ThreadController threadController;
		SendThread *sendThread;
		ReceiveThread *receiveThread;
		RfidThread *rfidThread;

	public:
		ArduinoDayGlobals *globals;
		void setup();
		void loop();
		void initHardware();
		void initThreadController();
		static void reiniciarSistema();
};

#endif