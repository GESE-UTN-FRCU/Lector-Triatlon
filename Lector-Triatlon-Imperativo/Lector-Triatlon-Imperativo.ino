//Librerias
#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <EtherCard.h>
#include <LiquidCrystal_SR_LCD3.h>
#include <DS3231.h>
#include <Thread.h>
#include <ThreadController.h>

// Caracteres agregados LCD.
#define CHAR_UTN 0

//Direcciones de la EEPROM ARDUINO
#define ADDR_MODO_CONFIG  0
#define ADDR_MYIP         2
#define ADDR_GWIP         6
#define ADDR_HISIP        10
#define ADDR_NETMASK      14
#define ADDR_HISPORT      18
#define ADDR_INDICE       19

//Pines al arduino NANO
#define PIN_BOTON   2
#define PIN_LCD_STROBE    3
#define PIN_LCD_DATA      4
#define PIN_LCD_CLOCK     5
#define PIN_LCD_LIGHT     6
#define PIN_MFRC522_RST   7
#define PIN_MFRC522_SDA   8
#define PIN_BUZZER        9
#define PIN_ETH_SDA       10

// Variables threads.
ThreadController threadController = new ThreadController();
Thread *rfidThread = new Thread();
Thread *ethernetThread = new Thread();
Thread *receiveThread = new Thread();
Thread *sendThread = new Thread();

// Variables Ethernet.
byte mymac[] = { 0x74,0x69,0x69,0xAA,0x30,0x20};
byte myip[] = { 192,168,8,152 };
byte gwip[] = { 192,168,8,1 };
byte hisip[] = { 192,168,8,222 };
byte dnsip[] = { 8,8,8,8 };
byte netmask[] = { 255,255,255,0 };
byte hisport=3000;
static byte session;
Stash stash;
byte Ethernet::buffer[514];

// Variables de la pantalla LCD.
LiquidCrystal_SR_LCD3 lcd(PIN_LCD_DATA, PIN_LCD_CLOCK, PIN_LCD_STROBE);
static byte logo_utn[8] = {0b10101, 0b10101, 0b01110, 0b11111, 0b01110, 0b10101, 0b10101};

// Variables de uso de la placa RFID.
MFRC522 rfid(PIN_MFRC522_SDA, PIN_MFRC522_RST);

// Ultima tarjeta RFID leida.
uint32_t ultimaLectura;

// Buffer para enviar datos.
char postBuffer[50];

// Variables de memoria Reloj
int indice=0;
const int tamano = 4;

// Pagina web de configuracion.
const char pagina[] PROGMEM =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<!DOCTYPE HTML>"
"<html>"
"<body>"
"<form action='config'>"
"<table>"
"<tr>"
"<td>Gateway</td>"
"<td>IP asignada</td>"
"<td>Mascara</td>"
"<td>IP del Servidor</td>"
"<td>Puerto</td>"
"</tr>"
"<tr>"
"<td><input name=gwip type=text></td>"
"<td><input name=myip type=text></td>"
"<td><input name=netmask type=text></td>"
"<td><input name=hisip type=text></td>"
"<td><input name=port type=number></td>"
"</tr>"
"</table>"
"<input type=submit value=Guardar>"
"</form>"
"</body>"
"</html>"
;

//Cuando se finaliza la configuracion se muestra esto
const char pagina_config[] PROGMEM =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"\r\n"
"Configuraci&oacute;n exitosa. No reinicie esta p&aacute;gina."
;

//-- AUX FUNCTIONS --//
// Reinicia el sistema por software.
void reiniciarSistema(){
  asm volatile("jmp 0");
}

template <typename T> String str_split(T* info, byte count, char separator){
  byte i;
  String aux;
  for (i=0; i < count - 1 ; i++) {
    aux += info[i];
    aux += separator;
  }
  aux += info[i];
  return aux;
}

//-- RFID --//
//Se fija si hay una lectura.
bool hayLectura(){
  // Chequea si hay una tarjeta para leer:
  if (!rfid.PICC_IsNewCardPresent()||!rfid.PICC_ReadCardSerial()) return false;
  // Chequea el tipo de tarjeta:
  MFRC522::PICC_Type piccType;
  piccType = rfid.PICC_GetType(rfid.uid.sak);
  if (piccType != MFRC522::PICC_TYPE_MIFARE_1K) {
    return false;
  }
  return true;
}

bool nuevaLectura(){
  if(hayLectura()){
    uint32_t codigo;
    memcpy((byte*) &codigo, &rfid.uid.uidByte, rfid.uid.size);
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    if(ultimaLectura!=codigo){
      memcpy(&ultimaLectura, &codigo, 4);
      return true;
    }
  }
  return false;
}

//-- LCD -- //
// Iniciar pantalla LCD.
static void iniciarLCD(){
  lcd.createChar(CHAR_UTN, logo_utn);
  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);
}

// Imprimir logo en la pantalla UTN.
static void iniciarLogoLCD(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(char(CHAR_UTN));
  lcd.print(F("UTN"));
}

// Cambiar linea de texto de la pantalla LCD.
static void cambiarLineaLCD(const char *linea){
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print(linea);
}

void apagarPantalla(uint32_t t) {
  uint16_t dt = t * 1000 / 256;
  for(byte i=255; i>0; i--){
    analogWrite(PIN_LCD_LIGHT, i);
    delayMicroseconds(dt);
  }
}

void prenderPantalla(uint32_t t) {
  uint16_t dt = t * 1000 / 256;
  for(byte i=0; i<255; i++){
    analogWrite(PIN_LCD_LIGHT, i);
    delayMicroseconds(dt);
  }
}

void imprimirIntento(byte intentos){
  lcd.setCursor(9,1);
  lcd.print(F("("));
  lcd.print(intentos);
  lcd.print(F(")..."));
  #if SERIAL_DEBUG
    Serial.println(F("Haciendo ping..."));
  #endif
}

//-- EEPROM --//
static void leerIndice(){
  int B;

  indice = EEPROM.read(ADDR_INDICE) << 8; //BYTE ALTO
  B = EEPROM.read(ADDR_INDICE + 1); //BYTE BAJO
  indice = indice | B;
}

static void guardarIndice(){
  byte H,L;

  H = highByte(indice);
  L = lowByte(indice);
  EEPROM.write(ADDR_INDICE, H);
  EEPROM.write(ADDR_INDICE + 1, L);
}

bool chequearModoConfig(){
  static bool verificado=false;
  static bool estado=false;
  if(!verificado)estado=EEPROM.read(ADDR_MODO_CONFIG);
  return estado;
}

// Cargar datos de memoria EEPROM.
void cargarDesdeEEPROM () {
  for(uint8_t i=0; i<4; i++){
    myip[i]=EEPROM.read(ADDR_MYIP+i);
    gwip[i]=EEPROM.read(ADDR_GWIP+i);
    hisip[i]=EEPROM.read(ADDR_HISIP+i);
    netmask[i]=EEPROM.read(ADDR_NETMASK+i);
  }
  hisport=EEPROM.read(ADDR_HISPORT);
}

void saveEthernetConfigEEPROM (byte myip[],byte gwip[], byte hisip[], byte netmask[], byte hisport) {
  for(uint8_t i=0; i<4; i++){
      EEPROM.write(ADDR_MYIP+i,myip[i]);
      EEPROM.write(ADDR_GWIP+i,gwip[i]);
      EEPROM.write(ADDR_HISIP+i,hisip[i]);
      EEPROM.write(ADDR_NETMASK+i,netmask[i]);
  }
  EEPROM.write(ADDR_HISPORT,hisport);
}

static void setModoConfig(bool estado) {
  EEPROM.write(ADDR_MODO_CONFIG,estado);
  Serial.print(F("Modo Config Cambiado"));
  delay(2000);
  reiniciarSistema();
}

//-- CLOCK/MEMORIA --//

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
void escribirLecturaMemoria(uint32_t tiempo, uint32_t codigo){
  byte buffer[8];
  
  memcpy(&buffer, &tiempo, 4);
  memcpy(&buffer[4], &codigo, 4);

  i2c_eeprom_write_page(0x57, 2 * indice*tamano, buffer,tamano);
  
  indice ++;
  guardarIndice();

}
//Esto anda mal
//Lee el ultimo tiempo en memoria (USA EL INDICE)
static uint32_t leerUltimoTiempo(){
    byte buffer[4];
    uint32_t bufferint;


    i2c_eeprom_read_buffer(0x57, 2 * indice*tamano, buffer, tamano);
    delay(10);

    memcpy(&bufferint,&buffer,4);

    return bufferint;
}

//Lee el ultimo codigo en memoria (USA EL INDICE)
static uint32_t leerUltimoCodigo(){ 
    byte buffer[4];
    uint32_t bufferint;

    i2c_eeprom_read_buffer(0x57, 2 * indice*tamano + indice*tamano, buffer, tamano);
    delay(10);

    memcpy(&bufferint,&buffer,4);
    return bufferint;
}

//Borra el ultimo codigo en memoria (USA EL INDICE)
static bool borrarUltimoCodigo(){
  if (indice >= 0){
    indice --;
    guardarIndice();
    return true;
  }
  return false;
}

//-- ETHERNET --//
// Chequear la conexion a una ip
// mediante el uso de pings.
bool chequearConexion(byte *ip,void (*callBack)(byte)){
  uint32_t timer=0;
  uint8_t intentos=1;

  while(intentos<=10){
    char len = ether.packetReceive();
    ether.packetLoop(len);

    // Si recibe algo del ping, se cierra el bucle.
    if (len > 0 && ether.packetLoopIcmpCheckReply(ip)) break;
    
    // Si pasaron mas de 5 segundos del ultimo intento,
    // intenta nuevamente.
    if (millis() - timer >= 5000) {
      callBack(intentos);
      ether.clientIcmpRequest(ip);
      timer = millis();
      intentos++;
    }
  }
  if(intentos<=10)return true;
  return false;
}

void imprimirConfiguracion(){
  ether.printIp("Mi IP: ", ether.myip);
  ether.printIp("Masc. de subred: ", ether.netmask);
  ether.printIp("IP del Gateway: ", ether.gwip);
  ether.printIp("IP del DNS: ", ether.dnsip);
  ether.printIp("IP del servidor: ", ether.hisip);
  Serial.print(F("Puerto del servidor: "));
  Serial.println(ether.hisport);
}

// Actualizar datos del dispositivo desde una URI.
// Nota: el formato de la uri es parametro1=valor1&parametro2=valor2
static void actualizarDatosDesdeURI(char* uri){
  char str[16];
  if(ether.findKeyVal(uri,str, sizeof str,"myip"))ether.parseIp(myip,str);
  if(ether.findKeyVal(uri,str, sizeof str,"gwip"))ether.parseIp(gwip,str);
  if(ether.findKeyVal(uri,str, sizeof str,"hisip"))ether.parseIp(hisip,str);
  if(ether.findKeyVal(uri,str, sizeof str,"netmask"))ether.parseIp(netmask,str);
  if(ether.findKeyVal(uri,str, sizeof str,"port"))hisport=atoi(str);
}

void routerHTTPConfig(char* cbuffer){
  if(strstr(cbuffer, "GET / ") != 0){
    memcpy_P(ether.tcpOffset(), pagina, sizeof pagina);
    ether.httpServerReply(sizeof pagina - 1);
  }else if(strstr(cbuffer, "GET /config?") != 0){
    actualizarDatosDesdeURI(cbuffer+6);
    memcpy_P(ether.tcpOffset(), pagina_config, sizeof pagina_config);
    ether.httpServerReply(sizeof pagina_config - 1);

    Serial.println(F("Nueva config:"));
    imprimirConfiguracion();
    
    saveEthernetConfigEEPROM(myip,gwip,hisip,netmask,hisport);

    //CONFIGURA EL INDICE EN 0 (BORRAR TODOS LOS DATOS)
    indice = 0;
    guardarIndice();
    setModoConfig(false);
  }
}

void routerHTTP(char* cbuffer){
  if(strstr(cbuffer, "GET /millis ") != 0){
    Serial.print(F("Enviando tiempo actual: "));
    sprintf(postBuffer,"m=%lu", millis);
    Serial.println(postBuffer);
    ether.httpPost(PSTR("/tiempo"), "192.168.8.127", NULL, postBuffer, NULL);
    Serial.println(F("Tiempo actual enviado."));
  }
}

void modoRouter(){  
  char pos = ether.packetLoop(ether.packetReceive());
  if(!pos)return;
  routerHTTP((char*)Ethernet::buffer + pos);
  }

void modoRouterConfig(){
  char pos = ether.packetLoop(ether.packetReceive());
  if(!pos)return;
  routerHTTPConfig((char*)Ethernet::buffer + pos);
  }

void enviarLectura(uint32_t millis, uint32_t codigo){

  Serial.println(F("Enviando lectura:"));
  sprintf(postBuffer,"m=%lu&c=%lu", millis, codigo);
  Serial.println(postBuffer);
  ether.httpPost(PSTR("/lectura"), "192.168.8.127", NULL, postBuffer, NULL);
  Serial.println(F("Lectura enviada."));
}

//-- THREAD CALLBACKS --//
void rfid_callback_function(){
  
    if(nuevaLectura()){

    uint32_t milisegundos = millis();
    
    Serial.println(F("Detectando tarjeta"));

    // Aca tendria que guardar una lectura en memoria (no funciona bien).
    //escribirLecturaMemoria(milisegundos,ultimaLectura);
    // Serial.println(leerUltimoCodigo());
    // Serial.println(leerUltimoTiempo());


    // Aca se manda la lectura.
    enviarLectura(milisegundos,ultimaLectura);
    
    cambiarLineaLCD("Leido");

    tone(PIN_BUZZER,880,500);
  }
  
  
  }

void web_callback_function(){
  modoRouter();
  }

void callback_function(){
  
  }


void initThreadController(){
  // Definir threads:
  rfidThread->setInterval(50);
  ethernetThread->setInterval(5);
  receiveThread->setInterval(300);
  sendThread->setInterval(1000);

  rfidThread->onRun(rfid_callback_function);
  ethernetThread->onRun(web_callback_function);
  receiveThread->onRun(callback_function);
  sendThread->onRun(callback_function);

  // Agregar threads:
  threadController.add(rfidThread);
  threadController.add(ethernetThread);
  threadController.add(receiveThread);
  threadController.add(sendThread);

}

void initPins(){
  //OUTPUT PINS
  pinMode(PIN_ETH_SDA, OUTPUT);
  digitalWrite(PIN_ETH_SDA, HIGH);
  
  pinMode(PIN_MFRC522_SDA, OUTPUT);
  digitalWrite(PIN_MFRC522_SDA, HIGH);
  
  pinMode(PIN_LCD_LIGHT, OUTPUT);
  analogWrite(PIN_LCD_LIGHT, 0);

  //INPUT PINS
  pinMode(PIN_BOTON, INPUT);
  
  }

void setup() {
  initPins();

  Wire.begin();
  Serial.begin(57600);
  SPI.begin();
  rfid.PCD_Init();  
  lcd.begin(16,2);

  iniciarLCD();
  iniciarLogoLCD();

  delay(2000);
  if (digitalRead(PIN_BOTON)) setModoConfig(HIGH);
  
  // Verificar que funcione correctamente la placa Ethernet.
  if (ether.begin(sizeof Ethernet::buffer, mymac, PIN_ETH_SDA) == 0){
      Serial.println(F("FailedEthernetController"));
      while(1);
  }

  prenderPantalla(2000);
  
  if(chequearModoConfig()){
      Serial.println(F("Modo config activado"));
      String aux = str_split(myip, 4, '.');
      cambiarLineaLCD(aux.c_str());
  }
  else{
      Serial.println(F("Modo config desactivado"));
      cambiarLineaLCD("Iniciando...");
      ultimaLectura = 0;
      // Cargar la configuracion desde la EEPROM.
      cargarDesdeEEPROM();
  }
  
  // Asignar la configuracion de la placa Ethernet usando
  // los valores estaticos extraidos de la EEPROM.
  ether.staticSetup(myip, gwip, dnsip, netmask);
  
  delay(1000);

  // Asignar la ip del servidor a la placa Ethernet.
  ether.copyIp(ether.hisip, hisip);
  //ACA DEBERIA METER EL HISPORT
  ether.hisport = hisport;
  
  imprimirConfiguracion();

  if(!chequearModoConfig()){
    // Si el sistema esta en MODO_CONFIG se chequea la conexion
    // haciendo ping al servidor.
    if(chequearConexion(ether.hisip,imprimirIntento)){
      // Si se verifica la conexion al servidor se muestra que el sistema
      // esta conectado y sigue funcionando correctamente.
      //while(ether.clientWaitingGw())ether.packetLoop(ether.packetReceive());
      cambiarLineaLCD("Conectado");
    }else{
      // Si el sistema no puede verificar la buena 
      cambiarLineaLCD("Verificar conex.");
      delay(2000);
      reiniciarSistema();
    }
  }
  else{
    while(chequearModoConfig())modoRouterConfig();
  }
  initThreadController();
}

void loop() {
  threadController.run();
}
