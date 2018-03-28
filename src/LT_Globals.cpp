#include <LT_Globals.h>

MFRC522 *Globals::rfid = new MFRC522(Globals::PIN_MFRC522_SDA, Globals::PIN_MFRC522_RST);
EtherCard *Globals::ethernet = &ether;
LiquidCrystal_SR_LCD3 *Globals::lcd = new LiquidCrystal_SR_LCD3(Globals::PIN_LCD_DATA, Globals::PIN_LCD_CLOCK, Globals::PIN_LCD_STROBE);