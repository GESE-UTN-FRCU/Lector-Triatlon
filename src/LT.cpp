#include <LT.h>


#define PIN_BOTON 		2

/*
ThreadController *LT::threadController = new ThreadController();
LT_RfidThread *LT::rfidThread = new LT_RfidThread();
LT_EthernetThread *LT::ethernetThread = new LT_EthernetThread();
LT_ReceiveThread *LT::receiveThread = new LT_ReceiveThread();
LT_SendThread *LT::sendThread = new LT_SendThread();
*/

static void LT::setup(){
	initHardware();
	//initThreadController();
}

static void LT::initHardware(){
	Serial.begin(Globals::SERIAL_FREQ);
	Serial.println(F("Iniciando sistema"));
	Wire.begin();
	SPI.begin();


	initPins();

	Serial.println(F("Iniciando Hardware Arduino."));

	LT_RFID::iniciarModulo();
	LT_Ethernet::iniciarModulo();
	//LT_LCD::iniciarModulo();

	//Setea el modo config en true en caso de estar presionado.
	delay(2000);
	if (digitalRead(PIN_BOTON)) {
		LT_MemoriaEEPROM::setModoConfig(true);
		LT::reiniciarSistema();
	};

	if (LT_MemoriaEEPROM::chequearModoConfig()) {

		Serial.println(F("Modo config activado."));
		
		while(LT_MemoriaEEPROM::chequearModoConfig())
		{
			LT_Ethernet::modoRouter();
		}

		Serial.println(F("Modo config desactivado."));
	}

	LT_Ethernet::imprimirConfiguracion();
	//LT_Ethernet::chequearConexion(Globals::ethernet->hisip);

	Serial.println(F("Iniciado con exito."));

}

static void LT::initPins(){
	// Inputs:
	pinMode(PIN_BOTON, INPUT);

}

static void LT::initThreadController(){
	/*
	// Definir threads:
	rfidThread->setInterval(100);
	ethernetThread->setInterval(5);
	//receiveThread->setInterval(300);
	sendThread->setInterval(1000);

	// Agregar threads:
	threadController->add(LT::rfidThread);
	threadController->add(LT::ethernetThread);
	//threadController->add(LT::receiveThread);
	threadController->add(LT::sendThread);
	*/
}

static void LT::loop(){
	//threadController->run();
}

// Reinicia el sistema por software.
static void LT::reiniciarSistema(){
	asm volatile("jmp 0");
}
