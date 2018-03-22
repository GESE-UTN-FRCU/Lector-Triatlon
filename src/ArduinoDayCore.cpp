#include "ArduinoDayCore.h"

const uint8_t ledPin = 8;
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
	this->sendThread = new Thread();
	this->sendThread->onRun(ArduinoDayThreads::thread_receive);
	this->sendThread->setInterval(100);

	this->receiveThread = new Thread();
	this->receiveThread->onRun(ArduinoDayThreads::thread_send);
	this->receiveThread->setInterval(1000);

	this->rfidThread = new Thread();
	this->rfidThread->onRun(ArduinoDayThreads::thread_rfid);
	this->rfidThread->setInterval(1000);

	//Add threads
	this->threadController.add(this->sendThread);
	this->threadController.add(this->receiveThread);
	this->threadController.add(this->rfidThread);
}

void ArduinoDayCore::loop(){
	this->threadController.run();
}

static void ArduinoDayThreads::thread_receive(){
	static bool ledStatus = false;
	ledStatus = !ledStatus;

	digitalWrite(ledPin, ledStatus);

	Serial.print("receive: ");
	Serial.println(millis());
}

static void ArduinoDayThreads::thread_send(){
	static bool ledStatus = false;
	ledStatus = !ledStatus;

	digitalWrite(ledPin2, ledStatus);

	Serial.print("send: ");
	Serial.println(millis());
}

static void ArduinoDayThreads::thread_rfid(){
	Serial.print("rfid: ");
	Serial.println(millis());
}