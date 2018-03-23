#ifndef _BASE_THREAD_H
#define _BASE_THREAD_H

#include <Arduino.h>
#include <Thread.h>
#include <ArduinoDayGlobals.h>

class BaseThread : public Thread{
	public:
		ArduinoDayGlobals *globals;
		BaseThread();
		void setGlobals(ArduinoDayGlobals* _globals);
};

#endif