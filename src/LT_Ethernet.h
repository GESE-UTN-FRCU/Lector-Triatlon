#ifndef _LT_ETHERNET_H_
#define _LT_ETHERNET_H_

#include <Arduino.h>
#include <LT_Globals.h>

class LT_Ethernet{
	private:
		static char pos;
		static word len;
	public:
		static void iniciarModulo();
		static void imprimirConfiguracion();
		static bool chequearConexion(byte *ip);
		static bool chequearConexion(byte *ip, void (*callBack)(byte));
		static void enviarJSON(char *url, JsonObject& data);
		static char recibirPaquetes();
		static char procesarPaquetes();
		static char punteroAlPaquete();
		static word TamanioDelPaquete();
};

#endif