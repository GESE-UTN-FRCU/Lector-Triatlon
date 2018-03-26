#ifndef _BASE_THREAD_H
#define _BASE_THREAD_H

#include <Arduino.h>
#include <ArduinoDayObject.h>
#include <Thread.h>

class BaseThread : public ArduinoDayObject, public Thread{
	public:
		BaseThread(ArduinoDayGlobals* _globals);
};

#endif