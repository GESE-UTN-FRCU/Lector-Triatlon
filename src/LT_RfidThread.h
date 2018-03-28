#ifndef _LT_RFID_THREAD_H
#define _LT_RFID_THREAD_H

#include <Arduino.h>
#include <LT_BaseThread.h>
#include <LT_RFID.h>

class LT_RfidThread : public LT_BaseThread{
	public:
		LT_RfidThread();
		bool shouldRun(long time);
		void run();
};

#endif