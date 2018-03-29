#ifndef _LT_MemoriaEEPROM_H_
#define _LT_MemoriaEEPROM_H_

#include <Arduino.h>
#include <EEPROM.h>
#include <LT_Globals.h>

class LT_MemoriaEEPROM{
	public:
		static void leerIndice();
		static void guardarIndice();
		bool chequearModoConfig();
		static void cargarDesdeEEPROM();
		static void guardarEnEEPROM();

};

#endif

