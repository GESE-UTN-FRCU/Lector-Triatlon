class RfidThread: public Thread{
	public:
		int pin;
		long duration;
		long _lastButtonPushed;
		ButtonThread(int _pin, long _duration): Thread(){
			// Set our attributes on construct
			pin = _pin;
			duration = _duration;
			_lastButtonPushed = 0;

			// Thread will start disabled
			enabled = false;

			// Configure the pin as INPUT and enable pull-up
			pinMode(pin, INPUT);
			digitalWrite(pin, HIGH);
		}

		/*
			Override the method responsible for
			checking if the thread should run.

			It will first check if the button is pressed.
			If so, we enable the thread, and then let the
			"Old" default Thread method 'shouldRun' return if
			it should run.
		*/
		bool shouldRun(long time){
			// Override enabled on thread when pin goes LOW.
			if(digitalRead(pin) == LOW){
				enabled = true;
				/*
					Here, we save the current time in this object,
					to compare it later.

					the 'time' parameter in this method, is an override for the
					'millis()' method. It allows who is checking the thread, to
					pass a custom time.

					This is sintax for writing an 'inline' if is very usefull, 
					it's the same as:
						if(time > 0){
							_lastButtonPushed = time;
						}else{
							_lastButtonPushed = millis();
						}
				*/
				_lastButtonPushed = (time ? time : millis());
			}

			// Let default method check for it.
			return Thread::shouldRun(time);
		}

		/*
			We 'disable' the thread after the duration on the
			'run' method.

			What we should do here, is check if the time saved
			in the _lastButtonPushed variable plus the duration,
			is greater than our current time. If that's true, it
			means we exceeded the thread time, and that we must
			disable it and prevent from running.
		*/
		void run(){
			// Check if time elapsed since last button push
			if(millis() > _lastButtonPushed + duration){
				// It exceeded time. We should disable it.
				enabled = false;
			}

			/*
				Run the thread.

				Note that this method will only get called
				from the ThreadList, IF the 'shouldRun' returns true.

				If the thread is not enabled anymore, it will run a 'last'
				time with the flag 'enabled' as false, meaning it's the last
				run in the period. You can use it for doing something only
				before it stops running.
			*/
			Thread::run();
		}
};