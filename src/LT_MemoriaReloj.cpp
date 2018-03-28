#include <LT_MemoriaReloj.h>

void i2c_eeprom_write_byte(int deviceaddress,unsigned int eeaddress,byte data){
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
}

void i2c_eeprom_write_page(int deviceaddress,unsigned int eeaddresspage,byte* data,byte length){
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddresspage >> 8)); // MSB
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB
  byte c;
  for ( c = 0; c < length; c++) Wire.write(data[c]);
  Wire.endTransmission();
}

byte i2c_eeprom_read_byte(int deviceaddress,unsigned int eeaddress){
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

void i2c_eeprom_read_buffer(int deviceaddress,unsigned int eeaddress,byte *buffer,int length){
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,length);
  int c = 0;
  for ( c = 0; c < length; c++ ) if (Wire.available()) buffer[c] = Wire.read();
}

// Guarda en memoria el ultimo indice.
bool LT_MemoriaReloj::escribirCodigoMemoria(byte codigo[9]){
  int addr=0,i;
  
  Serial.println(F("EscribiendoEnMemoria:"));
  for(i=0;i<9;i++){
    i2c_eeprom_write_byte(0x57,(Globals::indice*Globals::desplazamiento)+i,codigo[i]);
    Serial.print(codigo[i]);
    delay(10); //add a small delay
  }
  Serial.print('\n');
  return(Globals::desplazamiento==i); 
}

//Lee el ultimo codigo en memoria (USA EL INDICE)
bool LT_MemoriaReloj::leerCodigoMemoria(){
  int i,b;
  
  Serial.println("LeyendoEnMemoria:");
  for(i=0;i<9;i++){     
    Globals::data[i] = i2c_eeprom_read_byte(0x57, ((Globals::indice-1)*Globals::desplazamiento)+i); //access an address from the memory
    Serial.print(Globals::data[i]); 
    delay(10); 
  }
  Serial.println("");
  return(Globals::desplazamiento==i);
}
