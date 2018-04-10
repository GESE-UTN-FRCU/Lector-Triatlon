#ifndef _LT_ETHERNET_H_
#define _LT_ETHERNET_H_

#include <Arduino.h>
#include <LT_Globals.h>

static void gotPinged (byte* ptr) {
  ether.printIp(">>> ping from: ", ptr);
}

static byte session;
static Stash stash;

class LT_Ethernet{
	private:
	public:
		static void iniciarModulo();
		static void imprimirConfiguracion();
		static bool chequearConexion(byte *ip);
		static bool chequearConexion(byte *ip, void (*callBack)(byte));
		static void enviarAlgo();
		static void enviarJSON(char *method, char *url, JsonObject& data);
		static word recibirPaquetes();
		static word procesarPaquetes();
		static char* punteroAlPaquete();
		static word TamanioDelPaquete();
};

#endif