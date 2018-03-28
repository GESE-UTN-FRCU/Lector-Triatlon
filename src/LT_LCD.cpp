#include <LT_LCD.h>

// Iniciar pantalla LCD.
static void LT_LCD::iniciarModulo(){
	Globals::lcd->createChar(Globals::CHAR_UTN, LT_LCD::logo_utn);
	Globals::lcd->home();
	Globals::lcd->clear();
	Globals::lcd->setCursor(0,0);
}

// Imprimir logo en la pantalla UTN.
static void LT_LCD::iniciarLogoLCD(){
	Globals::lcd->clear();
	Globals::lcd->setCursor(0,0);
	Globals::lcd->print(char(Globals::CHAR_UTN));
	Globals::lcd->print(F("UTN"));
}

// Cambiar linea de texto de la pantalla LCD.
static void LT_LCD::cambiarLineaLCD(const char *linea){
	Globals::lcd->setCursor(0,1);
	Globals::lcd->print("                ");
	Globals::lcd->setCursor(0,1);
	Globals::lcd->print(linea);
}