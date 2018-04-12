#include <LT_SendThread.h>

LT_SendThread::LT_SendThread() : LT_BaseThread(){}

// Pregunta si deberia correr el hilo.
bool LT_SendThread::shouldRun(long time){
	return Thread::shouldRun(time);
}

const char website[] PROGMEM = "www.google.com";

// called when the client request is complete
static void my_callback (byte status, word off, word len) {
  Serial.println(">>>");
  Ethernet::buffer[off+300] = 0;
  Serial.print((const char*) Ethernet::buffer + off);
  Serial.println("...");
}

// Codigo en caso de correr.
void LT_SendThread::run(){
	// static uint16_t counter = 0;
	// Serial.print("SendThread: ");
	// Serial.println(counter++);
    //Globals::ethernet->browseUrl(PSTR("/foo/"), "bar", website, my_callback);

	this->runned();
}