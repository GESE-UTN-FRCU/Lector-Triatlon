#include <Thread.h>

int ledPin = 13;

// callback for myThread
void niceCallback(){
	static bool ledStatus = false;
	ledStatus = !ledStatus;

	digitalWrite(ledPin, ledStatus);

	Serial.print("COOL! I'm running on: ");
	Serial.println(millis());
}

class ArduinoDayCore {
	private:
		Thread myThread;
	public:
		void setup(){
			Serial.begin(9600);

			pinMode(ledPin, OUTPUT);

			myThread = Thread();
			myThread.onRun(niceCallback);
			myThread.setInterval(500);
		}
		void loop(){
			// checks if thread should run
			if(myThread.shouldRun())
				myThread.run();

			// Other code...
			int x = 0;
			x = 1 + 2;
		}
} Core;