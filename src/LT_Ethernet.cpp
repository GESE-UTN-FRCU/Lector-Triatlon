#include <LT_Ethernet.h>

// Iniciar placa ethernet.
static void LT_Ethernet::iniciarModulo(){
	/*
	¿Usaremos configuracion de ethernet?
	Asignar la configuracion de la placa Ethernet usando
	los valores estaticos extraidos de la EEPROM.
	*/
	Globals::ethernet->staticSetup(Globals::myip, Globals::gwip, Globals::dnsip, Globals::netmask);
	
	delay(1000);

	// Verificar que funcione correctamente la placa Ethernet.
	if (Globals::ethernet->begin(Globals::ETHERNET_BUFFER_SIZE, Globals::mymac, Globals::PIN_ETH_SDA) == 0){
		Serial.print("Error de Ethernet.");
		while(1);
	}
}