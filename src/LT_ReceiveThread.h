#ifndef _LT_RECEIVE_THREAD_H
#define _LT_RECEIVE_THREAD_H

#include <Arduino.h>
#include <LT_BaseThread.h>

class LT_ReceiveThread : public LT_BaseThread{
	public:
		LT_ReceiveThread();
		bool shouldRun(long time);
		void run();
};

#endif