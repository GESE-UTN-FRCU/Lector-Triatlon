#ifndef _RFID_H
#define _RFID_H

#include <Arduino.h>
#include <ArduinoDayObject.h>

class RFID : public ArduinoDayObject{
	public:
		bool chequearLectura();
		bool codigo_igual(byte codigo1[], byte codigo2[]);
};

#endif
