#include <LT_MemoriaReloj.h>

static void i2c_eeprom_write_byte(int deviceaddress,unsigned int eeaddress,byte data){
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
}

static void i2c_eeprom_write_page(int deviceaddress,unsigned int eeaddresspage,byte* data,byte length){
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddresspage >> 8)); // MSB
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB
  byte c;
  for ( c = 0; c < length; c++) Wire.write(data[c]);
  Wire.endTransmission();
}

static byte i2c_eeprom_read_byte(int deviceaddress,unsigned int eeaddress){
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

static void i2c_eeprom_read_buffer(int deviceaddress,unsigned int eeaddress,byte *buffer,int length){
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,length);
  int c = 0;
  for ( c = 0; c < length; c++ ) if (Wire.available()) buffer[c] = Wire.read();
}

// Guarda en memoria el ultimo indice.
static void LT_MemoriaReloj::escribirLecturaMemoria(uint32_t tiempo, uint32_t codigo){
  byte buffer[8];
  
  memcpy(&buffer, &tiempo, 4);
  memcpy(&buffer[4], &codigo, 4);

  i2c_eeprom_write_page(0x57, 2 * Globals::indice*Globals::tamano, buffer,Globals::tamano);
  
  Globals::indice ++;

}

//Lee el ultimo codigo en memoria (USA EL INDICE)
static uint32_t LT_MemoriaReloj::leerUltimoTiempo(){
    byte buffer[4];
    uint32_t bufferint;


    i2c_eeprom_read_buffer(0x57, 2 * Globals::indice * Globals::tamano, buffer, Globals::tamano);
    delay(10);

    memcpy(&bufferint,&buffer,4);

    return bufferint;
}

//Lee el ultimo codigo en memoria (USA EL INDICE)
static uint32_t LT_MemoriaReloj::leerUltimoCodigo(){ 
    byte buffer[4];
    uint32_t bufferint;

    i2c_eeprom_read_buffer(0x57, 2 * Globals::indice*Globals::tamano + Globals::indice*Globals::tamano, buffer, Globals::tamano);
    delay(10);

    memcpy(&bufferint,&buffer,4);
    return bufferint;
}

//Lee el ultimo codigo en memoria (USA EL INDICE)
static bool LT_MemoriaReloj::borrarUltimoCodigo(){
  if (Globals::indice >= 0){
    Globals::indice --;
    return true;
  }
  return false;
}