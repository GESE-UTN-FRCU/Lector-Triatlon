#ifndef _LT_ETHERNET_H_
#define _LT_ETHERNET_H_

#include <Arduino.h>
#include <LT_Globals.h>

class LT_Ethernet{
	private:
	public:
		static void iniciarModulo();
		static void imprimirConfiguracion();
		static bool chequearConexion(byte *ip);
		static bool chequearConexion(byte *ip, void (*callBack)(byte));
		static void enviarJSON(char *method, char *url, JsonObject& data);
		static word recibirPaquetes();
		static char* procesarPaquetes();
		static char* punteroAlPaquete();
		static word TamanioDelPaquete();
};

#endif
