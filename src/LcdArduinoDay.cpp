#include <LcdArduinoDay.h>

// Iniciar pantalla LCD.
void LcdArduinoDay::iniciarLcd(){
	lcd.createChar(CHAR_UTN, logo_utn);
	lcd.home();
	lcd.clear();
	lcd.setCursor(0,0);
}

// Imprimir logo en la pantalla UTN.
void LcdArduinoDay::iniciarLogoLCD(){
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(char(CHAR_UTN));
	lcd.print(F("UTN"));
}

// Cambiar linea de texto de la pantalla LCD.
void LcdArduinoDay::cambiarLineaLCD(const char *linea){
	lcd.setCursor(0,1);
	lcd.print("                ");
	lcd.setCursor(0,1);
	lcd.print(linea);
}
