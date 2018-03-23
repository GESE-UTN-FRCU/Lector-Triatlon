#ifndef _RFID_THREAD_H
#define _RFID_THREAD_H

#include <Arduino.h>
#include <Thread.h>


class RfidThread: public Thread{
	public:
		RfidThread();
		bool shouldRun(long time);
		void run();
};

#endif
