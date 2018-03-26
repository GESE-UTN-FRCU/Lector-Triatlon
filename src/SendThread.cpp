#include <SendThread.h>

SendThread::SendThread(ArduinoDayGlobals* _globals) : BaseThread(_globals){

}

// Pregunta si deberia correr el hilo.
bool SendThread::shouldRun(long time){
	return Thread::shouldRun(time);
}

// Codigo en caso de correr.
void SendThread::run(){
	static bool ledStatus = false;
	ledStatus = !ledStatus;
	digitalWrite(13, ledStatus);

	Serial.println("b");

	this->runned();
}
