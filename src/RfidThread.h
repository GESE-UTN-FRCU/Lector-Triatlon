#ifndef _RFID_THREAD_H
#define _RFID_THREAD_H

#include <Arduino.h>
#include <BaseThread.h>

class RfidThread : public BaseThread{
	public:
		bool shouldRun(long time);
		void run();
		bool chequearLectura();
};

#endif