#ifndef _ARDUINO_DAY_OBJECT_H_
#define _ARDUINO_DAY_OBJECT_H_

#include <ArduinoDayGlobals.h>

class ArduinoDayObject {
	public:
		ArduinoDayGlobals *globals;
		ArduinoDayObject();
		ArduinoDayObject(ArduinoDayGlobals* _globals);
		void setGlobals(ArduinoDayGlobals* _globals);
};

#endif