#ifndef _LT_ETHERNET_H_
#define _LT_ETHERNET_H_

#include <Arduino.h>
#include <EtherCard.h>
#include <LT_LCD.h>
#include <LT_MemoriaEEPROM.h>

class LT_Ethernet{
	private:
		static void actualizarDatosDesdeURI(char* uri);
	public:
		static void iniciarModulo();
		static void imprimirConfiguracion();
		static bool chequearConexion(byte *ip);
		static bool chequearConexion(byte *ip, void (*callBack)(byte));
		static void enviarLectura(uint32_t millis, uint32_t code);
		static char recibirPaquetes();
		static char procesarPaquetes();
		static char* punteroAlPaquete();
		static word TamanioDelPaquete();
		static void routerHTTP(char* cbuffer);
		static void modoRouter();
};

#endif