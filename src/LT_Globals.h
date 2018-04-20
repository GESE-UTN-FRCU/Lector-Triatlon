#ifndef _LT_GLOBALS_H_
#define _LT_GLOBALS_H_

#include <Arduino.h>

class LT_Globals {
    public:
		static char postBuffer[48];
		static uint32_t ultimaLectura;
		static uint8_t indice;

		static const uint8_t tamano = 4;
		static const uint32_t SERIAL_FREQ = 57600;
};

typedef LT_Globals Globals;

#endif
