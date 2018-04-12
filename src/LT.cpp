#include <LT.h>

ThreadController *LT::threadController = new ThreadController();
LT_RfidThread *LT::rfidThread = new LT_RfidThread();
LT_EthernetThread *LT::ethernetThread = new LT_EthernetThread();
LT_ReceiveThread *LT::receiveThread = new LT_ReceiveThread();
LT_SendThread *LT::sendThread = new LT_SendThread();

static void LT::setup(){
	initHardware();
	initThreadController();
}

static void LT::initHardware(){
	initPins();
	Wire.begin();
	Serial.begin(Globals::SERIAL_FREQ);
	while(!Serial);
	SPI.begin();

	Serial.println(F("Iniciando Hardware Arduino."));

	LT_RFID::iniciarModulo();
	LT_Ethernet::iniciarModulo();
	LT_LCD::iniciarModulo();

	LT_Ethernet::imprimirConfiguracion();
	LT_Ethernet::chequearConexion(Globals::ethernet->hisip);

	Serial.println(F("Iniciado con exito."));
}

static void LT::initPins(){
	// Inputs:
	pinMode(Globals::PIN_BOTON, INPUT);

	// Outputs:
	pinMode(Globals::PIN_ETH_SDA, OUTPUT);
	pinMode(Globals::PIN_MFRC522_SDA, OUTPUT);
	pinMode(Globals::PIN_LCD_LED, OUTPUT);
	
	digitalWrite(Globals::PIN_ETH_SDA, HIGH);
	digitalWrite(Globals::PIN_MFRC522_SDA, HIGH);
	analogWrite(Globals::PIN_LCD_LED, 1023);
}

static void LT::initThreadController(){
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
}

static void LT::loop(){
	threadController->run();
}

// Reinicia el sistema por software.
static void LT::reiniciarSistema(){
	asm volatile("jmp 0");
}
