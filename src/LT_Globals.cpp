#include <LT_Globals.h>

MFRC522 *Globals::rfid = new MFRC522(Globals::PIN_MFRC522_SDA, Globals::PIN_MFRC522_RST);