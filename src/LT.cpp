#include <LT.h>

ThreadController *LT::threadController = new ThreadController();
LT_SendThread *LT::sendThread = new LT_SendThread();
LT_ReceiveThread *LT::receiveThread = new LT_ReceiveThread();
LT_RfidThread *LT::rfidThread = new LT_RfidThread();

static void LT::setup(){
	initHardware();
	initThreadController();
}

static void LT::initHardware(){
	Serial.begin(Globals::SERIAL_FREQ);
	while (!Serial) continue;

	SPI.begin();
	Wire.begin();

	initPins();
	initRFID();
}

static void LT::initPins(){
	pinMode(Globals::PIN_BUZZER, OUTPUT);
	digitalWrite(Globals::PIN_BUZZER, LOW);

	pinMode(Globals::PIN_ETH_SDA, OUTPUT);
	digitalWrite(Globals::PIN_ETH_SDA, HIGH);

	pinMode(Globals::PIN_MFRC522_SDA, OUTPUT);
	digitalWrite(Globals::PIN_MFRC522_SDA, HIGH);
}

static void LT::initThreadController(){
	// Define threads:
	sendThread->setInterval(1000);
	rfidThread->setInterval(10);
	receiveThread->setInterval(3000);

	// Add threads:
	threadController->add(LT::rfidThread);
	threadController->add(LT::sendThread);
	threadController->add(LT::receiveThread);
}

static void LT::loop(){
	threadController->run();
}

// Reinicia el sistema por software.
static void LT::reiniciarSistema(){
	asm volatile("jmp 0");
}





// Las cosas de aca abajo se tienen que separar por cada componente...

static void LT::initEthernet(){
	Globals::ethernet = &ether;
	/*
	¿Usaremos configuracion de ethernet?
	Asignar la configuracion de la placa Ethernet usando
	los valores estaticos extraidos de la EEPROM.
	*/
	Globals::ethernet->staticSetup(Globals::myip, Globals::gwip, Globals::dnsip, Globals::netmask);
	
	delay(1000);

	// Verificar que funcione correctamente la placa Ethernet.
	if (Globals::ethernet->begin(Globals::ETHERNET_BUFFER_SIZE, Globals::mymac, Globals::PIN_ETH_SDA) == 0){
		Serial.print("Error de Ethernet.");
		while(1);
	}
}

static void LT::initRFID(){
	Globals::rfid->PCD_Init();
}

static void LT::initClock(){

}

static void LT::initLCD(){
	Globals::lcd = new LiquidCrystal_SR_LCD3(Globals::PIN_LCD_DATA, Globals::PIN_LCD_CLOCK, Globals::PIN_LCD_STROBE);
}
