#ifndef _LT_MemoriaReloj_H_
#define _LT_MemoriaReloj_H_

#include <Arduino.h>
#include <LT_Globals.h>

class LT_MemoriaReloj {
	public:
		bool escribirCodigoMemoria(byte codigo[9]);
		bool leerCodigoMemoria();
};

#endif
