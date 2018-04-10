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
	static uint32_t timer;

	while(true){
	  word len = Globals::ethernet->packetReceive(); // go receive new packets
	  word pos = Globals::ethernet->packetLoop(len); // respond to incoming pings
	  
	  // report whenever a reply to our outgoing ping comes back
	  if (len > 0 && Globals::ethernet->packetLoopIcmpCheckReply(Globals::ethernet->hisip)) {
	    Serial.print("  ");
	    Serial.print((micros() - timer) * 0.001, 3);
	    Serial.println(" ms");
	  	break;
	  }
	  
	  // ping a remote server once every few seconds
	  if (micros() - timer >= 5000000) {
	    Globals::ethernet->printIp("Pinging: ", Globals::ethernet->hisip);
	    timer = micros();
	    Globals::ethernet->clientIcmpRequest(Globals::ethernet->hisip);
	  }
	}

    //Globals::ethernet->browseUrl(PSTR("/foo/"), "bar", website, my_callback);

	this->runned();
}