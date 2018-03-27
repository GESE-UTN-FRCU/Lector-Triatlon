#ifndef _RFID_H
#define _RFID_H

#include <Arduino.h>

class RFID {
	public:
		static bool chequearLectura();
		static bool codigoIgual(byte codigo1, byte codigo2);
};

#endif
