#ifndef _LT_MemoriaReloj_H_
#define _LT_MemoriaReloj_H_

#include <Arduino.h>
#include <LT_Globals.h>

class LT_MemoriaReloj {
	public:
		static void escribirLecturaMemoria(uint32_t tiempo, uint32_t codigo);
		static uint32_t leerUltimoCodigo();
		static uint32_t leerUltimoTiempo();
		static bool borrarUltimoCodigo();
};

#endif
