#include "ArduinoDayCore.h"

void ArduinoDayCore::setup(){
	this->globals = new ArduinoDayGlobals();
	this->initHardware();
	this->initThreadController();
}

void ArduinoDayCore::initHardware(){
	Serial.begin(9600);
	while (!Serial) continue;

	pinMode(5, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(13, OUTPUT);

	this->globals->rfid = new MFRC522(this->globals->PIN_MFRC522_SDA, this->globals->PIN_MFRC522_RST);
	this->globals->ethernet = &ether;
}

void ArduinoDayCore::initThreadController(){
	this->threadController = ThreadController();

	// Define threads:
	this->sendThread = new SendThread();
	this->sendThread->setInterval(1000);

	this->rfidThread = new RfidThread();
	this->rfidThread->setGlobals(this->globals);
	this->rfidThread->setInterval(2000);

	this->receiveThread = new ReceiveThread();
	this->receiveThread->setInterval(3000);

	// Add threads:
	this->threadController.add(this->rfidThread);
	this->threadController.add(this->sendThread);
	this->threadController.add(this->receiveThread);
}

void ArduinoDayCore::loop(){
	this->threadController.run();
}

// Reinicia el sistema por software.
static void ArduinoDayCore::reiniciarSistema(){
	asm volatile("jmp 0");
}