#ifndef _LT_RFID_H
#define _LT_RFID_H

#include <Arduino.h>
#include <LT_Globals.h>

class LT_RFID{
	private:
		byte ultima_lectura[];
	public:
		static void iniciarModulo();
		static bool chequearLectura();
		static bool codigo_igual(byte codigo1[], byte codigo2[]);
};

#endif
