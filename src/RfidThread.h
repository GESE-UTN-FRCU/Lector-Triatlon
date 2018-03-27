#ifndef _RFID_THREAD_H
#define _RFID_THREAD_H

#include <Arduino.h>
#include <BaseThread.h>
#include <RFID.h>

class RfidThread : public BaseThread{
	private:
		RFID funcionesRFID;
	public:
		RfidThread(ArduinoDayGlobals* _globals);
		bool shouldRun(long time);
		void run();
		bool chequearLectura();
};

#endif
