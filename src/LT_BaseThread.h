#ifndef _LT_BASE_THREAD_H
#define _LT_BASE_THREAD_H

#include <Arduino.h>
#include <Thread.h>
#include <LT_Globals.h>

class LT_BaseThread : public Thread{
	public:
		LT_BaseThread();
};

#endif