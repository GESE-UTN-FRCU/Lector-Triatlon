#ifndef _LT_RFID_H
#define _LT_RFID_H

#include <Arduino.h>
#include <MFRC522.h>
#include <LT_Globals.h>

class LT_RFID{
	public:
		static void iniciarModulo();
		static bool hayLectura();
		static bool nuevaLectura();
		static bool compararCodigos(byte a[], byte b[]);
};

#endif
