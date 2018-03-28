#ifndef _LT_SEND_THREAD_H
#define _LT_SEND_THREAD_H

#include <Arduino.h>
#include <LT_BaseThread.h>

class LT_SendThread : public LT_BaseThread{
	public:
		LT_SendThread();
		bool shouldRun(long time);
		void run();
};

#endif