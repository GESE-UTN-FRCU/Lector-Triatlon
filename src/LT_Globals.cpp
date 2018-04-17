#include <LT_Globals.h>

byte Ethernet::buffer[ETH_BUFFER_SIZE];
MFRC522 *Globals::rfid = new MFRC522(Globals::PIN_MFRC522_SDA, Globals::PIN_MFRC522_RST);
EtherCard *Globals::ethernet = &ether;
LiquidCrystal_SR_LCD3 *Globals::lcd = new LiquidCrystal_SR_LCD3(Globals::PIN_LCD_DATA, Globals::PIN_LCD_CLOCK, Globals::PIN_LCD_STROBE);
uint32_t Globals::ultimaLectura = 0;
uint8_t Globals::indice = 0;
uint8_t Globals::mymac[6] = { 0x74,0x69,0x69,0xAA,0x30,0x20 };
uint8_t Globals::myip[4] = { 192,168,8,152 };
uint8_t Globals::gwip[4] = { 192,168,8,1 };
uint8_t Globals::hisip[4] = { 192,168,8,222 };
uint8_t Globals::dnsip[4] = { 8,8,8,8 };
uint8_t Globals::netmask[4] = { 255,255,255,0 };
uint8_t Globals::hisport = 3000;
char Globals::pos = 0;
char Globals::postBuffer[48];