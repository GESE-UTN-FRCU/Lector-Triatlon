#ifndef _ARDUINO_DAY_H_
#define _ARDUINO_DAY_H_

#include <Arduino.h>
#include <Thread.h>
#include <RfidThread.h>
#include <ThreadController.h>


class ArduinoDayCore {
	private:
		ThreadController threadController;
		RfidThread *rfidThread;

	public:
		void setup();
		void loop();
		void initHardware();
		void initThreadController();
};


#endif
