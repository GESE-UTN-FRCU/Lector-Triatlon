#include "BaseThread.h"

// Constructor del thread. Set de variables.
BaseThread::BaseThread(): Thread(){
	this->enabled = true;
}

void BaseThread::setGlobals(ArduinoDayGlobals* _globals){
	this->globals = _globals;
}