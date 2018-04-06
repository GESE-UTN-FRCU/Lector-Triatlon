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
	Wire.begin();
	Serial.begin(Globals::SERIAL_FREQ);
	SPI.begin();

	Serial.println("Iniciando arduino...");

	initPins();
	LT_RFID::iniciarModulo();
	LT_LCD::iniciarModulo();
	LT_Ethernet::iniciarModulo();
	LT_Ethernet::imprimirConfiguracion();

	Serial.println("Listo.");
}

static void LT::initPins(){
	pinMode(Globals::PIN_BUZZER, OUTPUT);
	digitalWrite(Globals::PIN_BUZZER, LOW);

	pinMode(Globals::PIN_ETH_SDA, OUTPUT);
	digitalWrite(Globals::PIN_ETH_SDA, HIGH);

	pinMode(Globals::PIN_MFRC522_SDA, OUTPUT);
	digitalWrite(Globals::PIN_MFRC522_SDA, HIGH);
}

static void LT::initThreadController(){
	// Define threads:
	rfidThread->setInterval(10);
	ethernetThread->setInterval(15);
	receiveThread->setInterval(300);
	sendThread->setInterval(500);

	// Add threads:
	threadController->add(LT::rfidThread);
	threadController->add(LT::ethernetThread);
	threadController->add(LT::receiveThread);
	threadController->add(LT::sendThread);
}

static void LT::loop(){
	threadController->run();
}

// Reinicia el sistema por software.
static void LT::reiniciarSistema(){
	asm volatile("jmp 0");
}
