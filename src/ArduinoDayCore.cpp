#include <ArduinoDayCore.h>

void ArduinoDayCore::setup(){
	this->initHardware();
	this->initThreadController();
}

void ArduinoDayCore::initHardware(){
	Serial.begin(this->globals->SERIAL_FREQ);
	while (!Serial) continue;

	SPI.begin();
	Wire.begin();

	this->initPins();
	//Iniciar los componentes

	this->initRFID();
}

void ArduinoDayCore::initPins(){
	pinMode(this->globals->PIN_BUZZER, OUTPUT);
	digitalWrite(this->globals->PIN_BUZZER, LOW);

	pinMode(this->globals->PIN_ETH_SDA, OUTPUT);
	digitalWrite(this->globals->PIN_ETH_SDA, HIGH);

	pinMode(this->globals->PIN_MFRC522_SDA, OUTPUT);
	digitalWrite(this->globals->PIN_MFRC522_SDA, HIGH);
}

void ArduinoDayCore::initThreadController(){
	this->threadController = ThreadController();

	// Define threads:
	this->sendThread = new SendThread(this->globals);
	this->sendThread->setInterval(1000);

	this->rfidThread = new RfidThread(this->globals);
	this->rfidThread->setInterval(10);

	this->receiveThread = new ReceiveThread(this->globals);
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





// Las cosas de aca abajo se tienen que separar por cada componente...

void ArduinoDayCore::initEthernet(){
	this->globals->ethernet = &ether;
	/*
	¿Usaremos configuracion de ethernet?
	Asignar la configuracion de la placa Ethernet usando
	los valores estaticos extraidos de la EEPROM.
	*/
	this->globals->ethernet->staticSetup(this->globals->myip, this->globals->gwip, this->globals->dnsip, this->globals->netmask);
	
	delay(1000);

	// Verificar que funcione correctamente la placa Ethernet.
	if (this->globals->ethernet->begin(this->globals->ETHERNET_BUFFER_SIZE, this->globals->mymac, this->globals->PIN_ETH_SDA) == 0){
		Serial.print("Error de Ethernet.");
		while(1);
	}
}

void ArduinoDayCore::initRFID(){
	this->globals->rfid = new MFRC522(this->globals->PIN_MFRC522_SDA, this->globals->PIN_MFRC522_RST);
	this->globals->rfid->PCD_Init();
}

void ArduinoDayCore::initClock(){

}

void ArduinoDayCore::initLCD(){
	this->globals->lcd = new LiquidCrystal_SR_LCD3(this->globals->PIN_LCD_DATA, this->globals->PIN_LCD_CLOCK, this->globals->PIN_LCD_STROBE);
}
