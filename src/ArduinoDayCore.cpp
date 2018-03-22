#include "ArduinoDayCore.h"

const uint8_t ledPin = 5;
const uint8_t ledPin2 = 8;

void ArduinoDayCore::setup(){
	this->initHardware();
	this->initThreads();
}

void ArduinoDayCore::initHardware(){
	Serial.begin(9600);

	pinMode(ledPin, OUTPUT);
	pinMode(ledPin2, OUTPUT);
}

void ArduinoDayCore::initThreads(){
	this->threadController = ThreadController();

	//Define threads
	this->myThread = Thread();
	this->myThread.onRun(ArduinoDayThreads::thread_receive);
	this->myThread.setInterval(100);

	this->myThread2 = Thread();
	this->myThread2.onRun(ArduinoDayThreads::thread_send);
	this->myThread2.setInterval(1000);

	//Add threads
	this->threadController.add(&this->myThread);
	this->threadController.add(&this->myThread2);
}

void ArduinoDayCore::loop(){
	this->threadController.run();
}

static void ArduinoDayThreads::thread_receive(){
	static bool ledStatus = false;
	ledStatus = !ledStatus;

	digitalWrite(ledPin, ledStatus);

	Serial.print("COOL! I'm running on: ");
	Serial.println(millis());
}

static void ArduinoDayThreads::thread_send(){
	static bool ledStatus = false;
	ledStatus = !ledStatus;

	digitalWrite(ledPin2, ledStatus);

	Serial.print("COOL! I'm running on: ");
	Serial.println(millis());
}

static void ArduinoDayThreads::thread_rfid(){

}