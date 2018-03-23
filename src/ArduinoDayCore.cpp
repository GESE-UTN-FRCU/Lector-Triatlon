#include "ArduinoDayCore.h"

const uint8_t ledPin = 13;
const uint8_t ledPin2 = 5;

void ArduinoDayCore::setup(){
	this->initHardware();
	this->initThreadController();
}

void ArduinoDayCore::initHardware(){
	Serial.begin(9600);

	pinMode(ledPin, OUTPUT);
	pinMode(ledPin2, OUTPUT);
}

void ArduinoDayCore::initThreadController(){
	this->threadController = ThreadController();

	//Define threads
	this->rfidThread = new RfidThread();
	this->rfidThread -> setInterval(500);


	//Add threads
	this->threadController.add(this->rfidThread);
}

void ArduinoDayCore::loop(){
	this->threadController.run();
}
