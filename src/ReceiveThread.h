#ifndef _RECEIVE_THREAD_H
#define _RECEIVE_THREAD_H

#include <Arduino.h>
#include <BaseThread.h>

class ReceiveThread : public BaseThread{
	public:
		bool shouldRun(long time);
		void run();
};

#endif