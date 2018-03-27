#include <LcdArduinoDay.h>

// Iniciar pantalla LCD.
void LcdArduinoDay::iniciarLcd(){
	this->globals->lcd->createChar(this->globals->CHAR_UTN, logo_utn);
	this->globals->lcd->home();
	this->globals->lcd->clear();
	this->globals->lcd->setCursor(0,0);
}

// Imprimir logo en la pantalla UTN.
void LcdArduinoDay::iniciarLogoLCD(){
	this->globals->lcd->clear();
	this->globals->lcd->setCursor(0,0);
	this->globals->lcd->print(char(this->globals->CHAR_UTN));
	this->globals->lcd->print(F("UTN"));
}

// Cambiar linea de texto de la pantalla LCD.
void LcdArduinoDay::cambiarLineaLCD(const char *linea){
	this->globals->lcd->.setCursor(0,1);
	this->globals->lcd->print("                ");
	this->globals->lcd->setCursor(0,1);
	this->globals->lcd->print(linea);
}
