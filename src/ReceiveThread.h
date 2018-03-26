#ifndef _RECEIVE_THREAD_H
#define _RECEIVE_THREAD_H

#include <Arduino.h>
#include <BaseThread.h>

class ReceiveThread : public BaseThread{
	public:
		ReceiveThread(ArduinoDayGlobals* _globals);
		bool shouldRun(long time);
		void run();
};

#endif