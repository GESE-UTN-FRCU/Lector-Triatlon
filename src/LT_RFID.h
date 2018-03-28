#ifndef _LT_RFID_H
#define _LT_RFID_H

#include <Arduino.h>
#include <LT_Globals.h>

class LT_RFID{
	private:
		static uint32_t ultimaLectura;
	public:
		static void iniciarModulo();
		static bool hayLectura();
		static bool nuevaLectura();
		static uint32_t getUltimaLectura();
		static bool compararCodigos(byte a[], byte b[]);
};

#endif