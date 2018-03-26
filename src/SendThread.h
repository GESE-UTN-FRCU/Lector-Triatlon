#ifndef _SEND_THREAD_H
#define _SEND_THREAD_H

#include <Arduino.h>
#include <BaseThread.h>

class SendThread : public BaseThread{
	public:
		SendThread(ArduinoDayGlobals* _globals);
		bool shouldRun(long time);
		void run();
};

#endif