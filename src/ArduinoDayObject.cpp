#include <ArduinoDayObject.h>

ArduinoDayObject::ArduinoDayObject(){
	this->setGlobals(new ArduinoDayGlobals());
}

ArduinoDayObject::ArduinoDayObject(ArduinoDayGlobals* _globals) : globals(_globals){
}

void ArduinoDayObject::setGlobals(ArduinoDayGlobals* _globals){
	this->globals = _globals;
}