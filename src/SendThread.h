#ifndef _SEND_THREAD_H
#define _SEND_THREAD_H

#include <Arduino.h>
#include <BaseThread.h>

class SendThread : public BaseThread{
	public:
		bool shouldRun(long);
		void run();
};

#endif