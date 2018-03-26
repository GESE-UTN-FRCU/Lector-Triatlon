#include <BaseThread.h>

BaseThread::BaseThread(ArduinoDayGlobals* _globals) : Thread(), ArduinoDayObject(_globals){
	this->enabled = true;
}