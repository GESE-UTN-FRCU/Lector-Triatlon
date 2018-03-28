#ifndef _LT_ETHERNET_THREAD_H
#define _LT_ETHERNET_THREAD_H

#include <Arduino.h>
#include <LT_BaseThread.h>
#include <LT_Ethernet.h>

class LT_EthernetThread : public LT_BaseThread{
	public:
		LT_EthernetThread();
		bool shouldRun(long time);
		void run();
};

#endif