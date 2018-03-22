#ifndef _ARDUINO_DAY_H_
#define _ARDUINO_DAY_H_

#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>

class ArduinoDayCore {
	private:
		ThreadController threadController;
		Thread myThread, myThread2;

	public:
		void setup();
		void loop();
		void initHardware();
		void initThreads();
};

class ArduinoDayThreads {
	public:
		static void thread_receive();
		static void thread_send();
		static void thread_rfid();
};

#endif