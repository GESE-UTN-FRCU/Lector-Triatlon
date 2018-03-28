#include <LT.h>

ThreadController *LT::threadController = new ThreadController();
LT_SendThread *LT::sendThread = new LT_SendThread();
LT_ReceiveThread *LT::receiveThread = new LT_ReceiveThread();
LT_RfidThread *LT::rfidThread = new LT_RfidThread();

static void LT::setup(){
	initHardware();
	initThreadController();
}

static void LT::initHardware(){
	Serial.begin(Globals::SERIAL_FREQ);
	while (!Serial) continue;

	SPI.begin();
	Wire.begin();

	initPins();
	LT_RFID::iniciarModulo();
	//LT_LCD::iniciarModulo();
	//LT_Ethernet::iniciarModulo();
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
	sendThread->setInterval(1000);
	rfidThread->setInterval(10);
	receiveThread->setInterval(3000);

	// Add threads:
	threadController->add(LT::rfidThread);
	threadController->add(LT::sendThread);
	threadController->add(LT::receiveThread);
}

static void LT::loop(){
	threadController->run();
}

// Reinicia el sistema por software.
static void LT::reiniciarSistema(){
	asm volatile("jmp 0");
}