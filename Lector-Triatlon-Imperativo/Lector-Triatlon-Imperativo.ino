//Librerias
#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <EtherCard.h>
#include <LiquidCrystal_SR_LCD3.h>
#include <DS3231.h>

// Caracteres agregados LCD.
#define CHAR_UTN 0

//Direcciones de la EEPROM ARDUINO
#define ADDR_MODO_CONFIG  0
#define ADDR_MYIP         2
#define ADDR_GWIP         6
#define ADDR_HISIP        10
#define ADDR_NETMASK      14
#define ADDR_HISPORT      18
#define ADDR_INDICE       20

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

// Variables Ethernet.
static byte mymac[] = { 0x74,0x69,0x69,0xAA,0x30,0x20};
static byte myip[] = { 192,168,10,152 };
static byte gwip[] = { 192,168,10,1 };
static byte hisip[] = { 192,168,10,127 };
static byte dnsip[] = { 8,8,8,8 };
static byte netmask[] = { 255,255,255,0 };
static uint16_t hisport = 80;
static byte session;
Stash stash;
byte Ethernet::buffer[450];

// Variables de la pantalla LCD.
LiquidCrystal_SR_LCD3 lcd(PIN_LCD_DATA, PIN_LCD_CLOCK, PIN_LCD_STROBE);
static byte logo_utn[8] = {0b10101, 0b10101, 0b01110, 0b11111, 0b01110, 0b10101, 0b10101};

// Variables del timer LCD y BUZZER
uint32_t millisPrevios = 0;
bool listoLectura = false;
uint32_t millisBuzzer = 0;
int contadorBip = 3;

// Variables de envio de lectura.
bool modoEnvioDatos = false;
bool lecturaEnviada = false;
uint16_t intentosEnvio = 0;
uint32_t millisEnvio = 0;

// Variables de uso de la placa RFID.
MFRC522 rfid(PIN_MFRC522_SDA, PIN_MFRC522_RST);

// Ultima tarjeta RFID leida.
uint32_t ultimaLectura;

// Variables de memoria Reloj
int indice=0;
const int tamano = 4;

// Pagina web de configuracion.
const char pagina[] PROGMEM =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"\r\n"
"<!DOCTYPE HTML>"
"<html>"
"<body>"
"<form action='config'>"
"<input name=gwip placeholder='IPGW' type=text>"
"<input name=myip placeholder='IPDIS' type=text>"
"<input name=netmask placeholder='NETMASK' type=text>"
"<input name=hisip placeholder='IPSER' type=text>"
"<input name=port placeholder='PORT' type=number>"
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
  Serial.println(F("Haciendo ping..."));
}

//-- BUZZER -- //
void hacerBip()
{
        if (millis() - millisBuzzer > 50 && contadorBip < 3)
        {
          if (digitalRead(PIN_BUZZER, LOW))
          {
            digitalWrite(PIN_BUZZER, HIGH);
          }
          else {digitalWrite(PIN_BUZZER, LOW)}
          contadorBip++;
          millisBuzzer = millis();
        }
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
  int B;
  for(uint8_t i=0; i<4; i++){
    myip[i]=EEPROM.read(ADDR_MYIP+i);
    gwip[i]=EEPROM.read(ADDR_GWIP+i);
    hisip[i]=EEPROM.read(ADDR_HISIP+i);
    netmask[i]=EEPROM.read(ADDR_NETMASK+i);
  }
  hisport = EEPROM.read(ADDR_HISPORT) << 8;
  B = EEPROM.read(ADDR_HISPORT + 1);
  hisport = hisport | B;
}

void saveEthernetConfigEEPROM (byte myip[],byte gwip[], byte hisip[], byte netmask[], uint16_t hisport) {
  byte H,L;
  for(uint8_t i=0; i<4; i++){
      EEPROM.write(ADDR_MYIP+i,myip[i]);
      EEPROM.write(ADDR_GWIP+i,gwip[i]);
      EEPROM.write(ADDR_HISIP+i,hisip[i]);
      EEPROM.write(ADDR_NETMASK+i,netmask[i]);
  }
  H = highByte(hisport);
  L = lowByte(hisport);
  EEPROM.write(ADDR_HISPORT, H);
  EEPROM.write(ADDR_HISPORT + 1, L);
}

static void setModoConfig(bool estado) {
  EEPROM.write(ADDR_MODO_CONFIG,estado);
  Serial.println(F("Modo config cambiado"));
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

  i2c_eeprom_write_page(0x57, 2*indice*tamano, buffer,2*tamano);

  indice++;
  guardarIndice();

}
//Lee el ultimo tiempo en memoria (USA EL INDICE)
static uint32_t leerUltimoTiempo(){
    byte buffer[4];
    uint32_t bufferint;

    i2c_eeprom_read_buffer(0x57, 2*(indice-1)*tamano, buffer, tamano);
    delay(10);

    memcpy(&bufferint,&buffer,4);

    return bufferint;
}

//Lee el ultimo codigo en memoria (USA EL INDICE)
static uint32_t leerUltimoCodigo(){
    byte buffer[4];
    uint32_t bufferint;

    i2c_eeprom_read_buffer(0x57, 2*(indice-1)*tamano + tamano, buffer, tamano);
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
// Chequear la conexion a una ip mediante el uso de pings.
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
  Serial.println(hisport);
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

static word homePageMillis() {
 char millisChar[16];
 sprintf(millisChar,"%lu", millis());
 BufferFiller bfill = ether.tcpOffset();
 bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n"
      "Content-Type: text/html\r\n"
      "\r\n"
      "$S"
      ),millisChar);

  return bfill.position();
}

static word homePageLectura() {
 char ultimaLecturaChar[16];
 sprintf(ultimaLecturaChar,"%lu", ultimaLectura);
 BufferFiller bfill = ether.tcpOffset();
 bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n"
      "Content-Type: text/html\r\n"
      "\r\n"
      "$S"
      ),ultimaLecturaChar);

  return bfill.position();
}

static word homePageDato() {

 char modoEnvioDatosChar[5] = "";
 if(modoEnvioDatos) {
  strncpy(modoEnvioDatosChar,"TRUE",5);
 }
 else {
  strncpy(modoEnvioDatosChar,"FALSE",5);
 };

 BufferFiller bfill = ether.tcpOffset();
 bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n"
      "Content-Type: text/html\r\n"
      "\r\n"
      "$S"
      ),modoEnvioDatosChar);

  return bfill.position();
}


void routerHTTP(char* cbuffer){
  if(strstr(cbuffer, "GET /millis") != 0){

    Serial.print(F("Enviando tiempo actual: "));
    Serial.println(millis());
    ether.httpServerReply(homePageMillis());
    Serial.println(F("Tiempo actual enviado."));
  }
  else if(strstr(cbuffer, "GET /lectura") != 0){

    Serial.print(F("Enviando ultima lectura: "));
    Serial.println(ultimaLectura);
    ether.httpServerReply(homePageLectura());
    Serial.println(F("Ultima lectura enviada."));
  }
  else if(strstr(cbuffer, "GET /modoenviodato") != 0){

    modoEnvioDatos = !modoEnvioDatos;

    Serial.print(F("Modo envio datos cambiado."));
    ether.httpServerReply(homePageDato());
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

static void enviarLectura(uint32_t milisegundos, uint32_t codigo){
  // Cambia el puerto al del servidor
  ether.hisport = hisport;

  byte sd = stash.create();

  // Imprime el tiempo en el Stash.
  stash.print("m=");
  stash.print(milisegundos);
  // Imprime el codigo en el Stash.
  stash.print("&c=");
  stash.print(codigo);

  stash.save();

  // Formatea el Stash como una peticion POST de HTTP.
  Stash::prepare(PSTR(
    "POST http://$D.$D.$D.$D/$S HTTP/1.1" "\r\n"
    "Content-Type: application/x-www-form-urlencoded" "\r\n"
    "Content-Length: $D" "\r\n"
    "\r\n"
    "$H"),
  hisip[0], hisip[1], hisip[2], hisip[3], "lectura", stash.size(), sd);

  session = ether.tcpSend();
  Serial.println(F("Lectura enviada."));

  // Cambia el puerto al 80 para acceder a los gets.
  ether.hisport = 80;

}


//-- Funciones principales --//
void rfid_callback_function(){

    //Borra el "Leído" de la pantalla
    if (millis() - millisPrevios > 1000 && !listoLectura)
    {
        cambiarLineaLCD("Listo para leer");
        listoLectura = true;
    };

    if(nuevaLectura()){

      uint32_t milisegundos = millis();

      Serial.println(F("Detectando tarjeta"));

      // Muestra en el puerto serial lo leido.
      Serial.print(F("Milisegundos: "));
      Serial.println(milisegundos);
      Serial.print(F("Codigo: "));
      Serial.println(ultimaLectura);

      // Guarda la lectura en memoria.
      escribirLecturaMemoria(milisegundos,ultimaLectura);
      Serial.print(F("Milisegundos en memoria: "));
      Serial.println(leerUltimoTiempo());
      Serial.print(F("Codigo en memoria: "));
      Serial.println(leerUltimoCodigo());

      // Envia a el servidor lo leido.
      enviarLectura(milisegundos,ultimaLectura);

      // Muestra en pantalla los cambios.
      cambiarLineaLCD("Leido");

      // Asigna los valores para el correcto funcionamiento del timer y la funcion del buzzer
      listoLectura = false;
      millisPrevios = millis();
      contadorBip = 0;
    }

    // Hace sonido cuando hay una lectura
    hacerBip();

  }

void web_callback_function(){
  modoRouter();
  }

void data_callback_function(){

  if (!lecturaEnviada){
    enviarLectura(leerUltimoTiempo(),leerUltimoCodigo());
    lecturaEnviada = true;
  }

  const char* reply = ether.tcpReply(session);
  if (reply != 0) {
    if(strstr(reply,"OK")!=0){
            borrarUltimoCodigo();
            lecturaEnviada = false;
          }
    Serial.println(reply);
  }else{
    if (millis() - millisEnvio > 1000) {
    intentosEnvio++;
    millisEnvio = millis();
    if (intentosEnvio == 10){
        intentosEnvio = 0;
        modoEnvioDatos = false;
        lecturaEnviada = false;
        Serial.println(F("Modo envio de datos desactivado."));
      }
    }

  }
}


// SETUP
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
      Serial.println(F("EthernetController fallo"));
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
      leerIndice();

  };

  // Asignar la configuracion de la placa Ethernet usando
  // los valores estaticos extraidos de la EEPROM.
  ether.staticSetup(myip, gwip, dnsip, netmask);

  delay(1000);

  // Asignar la ip del servidor a la placa Ethernet.
  ether.copyIp(ether.hisip, hisip);

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
}

void loop() {
  //Funcion principal del RFID.
  rfid_callback_function();

  //Funcion principal de la web para obtener los get.
  web_callback_function();

  //Funcion principal en modo envio de datos.
  while (indice > 0 && modoEnvioDatos) {
    web_callback_function();
    data_callback_function();
  }
}
