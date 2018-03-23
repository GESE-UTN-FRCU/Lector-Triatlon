#include "ArduinoDayCore.h"

void ArduinoDayCore::setup(){
	this->globals = new ArduinoDayGlobals();
	this->initHardware();
	this->initThreadController();
}

void ArduinoDayCore::initHardware(){
	Serial.begin(57600);
	while (!Serial) continue;

	SPI.begin();
	Wire.begin();

	pinMode(this->globals->PIN_BUZZER, OUTPUT);
	digitalWrite(this->globals->PIN_BUZZER,LOW);

	pinMode(this->globals->PIN_ETH_SDA, OUTPUT);
	digitalWrite(this->globals->PIN_ETH_SDA,HIGH);

	pinMode(this->globals->PIN_MFRC522_SDA, OUTPUT);
	digitalWrite(this->globals->PIN_MFRC522_SDA, HIGH);

	this->globals->rfid = new MFRC522(this->globals->PIN_MFRC522_SDA, this->globals->PIN_MFRC522_RST);
	this->globals->ethernet = &ether;

	this->globals->rfid->PCD_Init();

	/*
	// Verificar que funcione correctamente la placa Ethernet.
	if (this->globals->ethernet->begin(sizeof Ethernet::buffer, this->globals->mymac, this->globals->PIN_ETH_SDA) == 0){
		Serial.Begin("Error de Ethernet.");
		while(1);
	}
	*/
}

void ArduinoDayCore::initThreadController(){
	this->threadController = ThreadController();

	// Define threads:
	this->sendThread = new SendThread();
	this->sendThread->setInterval(1000);

	this->rfidThread = new RfidThread();
	this->rfidThread->setGlobals(this->globals);
	this->rfidThread->setInterval(10);

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