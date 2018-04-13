#include <LT_MemoriaEEPROM.h>

static void LT_MemoriaEEPROM::leerIndice(){
  int A,B;

  A = EEPROM.read(Globals::ADDR_INDICE) << 8; //BYTE ALTO
  B = EEPROM.read(Globals::ADDR_INDICE + 1); //BYTE BAJO
  Globals::indice = A | B;
}

static void LT_MemoriaEEPROM::guardarIndice(){
  byte H,L;

  H = highByte(Globals::indice);
  L = lowByte(Globals::indice);
  EEPROM.write(Globals::ADDR_INDICE, H);
  EEPROM.write(Globals::ADDR_INDICE + 1, L);
}

static void LT_MemoriaEEPROM::setModoConfig(bool modo){
    EEPROM.write(Globals::ADDR_MODO_CONFIG,modo);
}

static bool LT_MemoriaEEPROM::chequearModoConfig(){
  static bool verificado=false;
  static bool estado=false;
  if(!verificado)estado=EEPROM.read(Globals::ADDR_MODO_CONFIG);
  return estado;
}

// Cargar datos de memoria EEPROM.
static void LT_MemoriaEEPROM::cargarDesdeEEPROM() {
  for(uint8_t i=0; i<4; i++){
    Globals::myip[i]=EEPROM.read(Globals::ADDR_MYIP+i);
    Globals::gwip[i]=EEPROM.read(Globals::ADDR_GWIP+i);
    Globals::hisip[i]=EEPROM.read(Globals::ADDR_HISIP+i);
    Globals::netmask[i]=EEPROM.read(Globals::ADDR_NETMASK+i);
  }
  Globals::hisport=EEPROM.read(Globals::ADDR_HISPORT);
}

static void LT_MemoriaEEPROM::guardarEnEEPROM() {
  for(uint8_t i=0; i<4; i++){
      EEPROM.write(Globals::ADDR_MYIP+i,Globals::myip[i]);
      EEPROM.write(Globals::ADDR_GWIP+i,Globals::gwip[i]);
      EEPROM.write(Globals::ADDR_HISIP+i,Globals::hisip[i]);
      EEPROM.write(Globals::ADDR_NETMASK+i,Globals::netmask[i]);
  }
  EEPROM.write(Globals::ADDR_HISPORT,Globals::hisport);
}
