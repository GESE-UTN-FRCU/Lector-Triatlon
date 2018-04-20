#include <LT_LCD.h>

#define PIN_LCD_STROBE 	3
#define PIN_LCD_DATA 	4
#define PIN_LCD_CLOCK 	5
#define PIN_LCD_LED 	6

//Variables del LCD
LiquidCrystal_SR_LCD3 lcd(PIN_LCD_DATA, PIN_LCD_CLOCK, PIN_LCD_STROBE);

// Iniciar pantalla LCD.
static void LT_LCD::iniciarModulo(){

	pinMode(PIN_LCD_LED, OUTPUT);

	lcd.begin(16,2);
	//Globals::lcd->createChar(Globals::CHAR_UTN, LT_LCD::logo_utn);
	lcd.home();
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Lector-Triatlon");
	analogWrite(PIN_LCD_LED, 1023);
}

// Imprimir logo en la pantalla UTN.
static void LT_LCD::iniciarLogoLCD(){
	lcd.clear();
	lcd.setCursor(0,0);
	// lcd.print(char(Globals::CHAR_UTN));
	lcd.print(F("UTN"));
}

// Cambiar linea de texto de la pantalla LCD.
static void LT_LCD::cambiarLineaLCD(const char *linea){
	lcd.setCursor(0,1);
	lcd.print("                ");
	lcd.setCursor(0,1);
	lcd.print(linea);
}