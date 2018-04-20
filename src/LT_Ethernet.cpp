#include <LT_Ethernet.h>

#define PIN_ETH_SDA   10

static const char pagina[] PROGMEM =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"<!DOCTYPE HTML>"
"<html>"
"<body>"
"<form action='config'>"
"<table>"
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
static const char pagina_config[] PROGMEM =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"\r\n"
"Configuraci&oacute;n exitosa. No reinicie esta p&aacute;gina."
;

static const char HOST[] PROGMEM = "192.168.8.127:3000";

// Variables Ethernet.
byte mymac[] = { 0x74,0x69,0x69,0xAA,0x30,0x20};
byte myip[] = { 192,168,8,152 };
byte gwip[] = { 192,168,8,1 };
byte hisip[] = { 192,168,8,222 };
byte dnsip[] = { 8,8,8,8 };
byte netmask[] = { 255,255,255,0 };
byte hisport = 3000;
static byte session;
Stash stash;
byte Ethernet::buffer[514];

void gotPinged(byte* ptr) {
  ether.printIp(">>> ping from: ", ptr);
}


// Iniciar placa ethernet.
static void LT_Ethernet::iniciarModulo(){

  pinMode(PIN_ETH_SDA, OUTPUT);
  digitalWrite(PIN_ETH_SDA, HIGH);

  // Verificar que funcione correctamente la placa Ethernet.
  if (ether.begin(sizeof Ethernet::buffer, mymac, PIN_ETH_SDA) == 0){
    Serial.println(F("Error de Ethernet."));
    while(1);
  }
  
  //ACA DEBERIA CARGAR ESTO DE MEMORIA):
  if (LT_MemoriaEEPROM::chequearModoConfig()){
    ether.staticSetup(myip, gwip, dnsip, netmask);
    ether.copyIp(ether.hisip, hisip);
  }
  else{
    //LT_MemoriaEEPROM::cargarDesdeEEPROM(); 
    ether.staticSetup(myip, gwip, dnsip, netmask); 
    ether.copyIp(ether.hisip, hisip);
  }

  delay(1000);

  ether.hisport = 3000;
  ether.registerPingCallback(gotPinged);

  LT_Ethernet::imprimirConfiguracion();
}

static void LT_Ethernet::imprimirConfiguracion(){
  ether.printIp("Mi IP: ", ether.myip);
  ether.printIp("Masc. de subred: ", ether.netmask);
  ether.printIp("IP del Gateway: ", ether.gwip);
  ether.printIp("IP del DNS: ", ether.dnsip);
  ether.printIp("IP del servidor: ", ether.hisip);
  Serial.print("Puerto del servidor: ");
  Serial.println(ether.hisport);
}

static bool LT_Ethernet::chequearConexion(byte *ip){

  uint32_t timer=0;
  uint8_t intentos=1;

  while(intentos<=10){
    word len = ether.packetReceive();
    ether.packetLoop(len);

    // Si recibe algo del ping, se cierra el bucle.
    if (len > 0 && ether.packetLoopIcmpCheckReply(ip)) break;
    
    // Si pasaron mas de 5 segundos del ultimo intento,
    // intenta nuevamente.
    if (millis() - timer >= 2000) {

      Serial.println(F("Haciendo ping al host."));

      ether.clientIcmpRequest(ip);
      timer = millis();
      intentos++;
    }
  }
  if(intentos<=10)return true;
  return false;

}

static void LT_Ethernet::enviarLectura(uint32_t millis, uint32_t codigo){

  Serial.println(F("Enviando lectura:"));

  sprintf(Globals::postBuffer,"m=%lu&c=%lu", millis, codigo);
  Serial.println(Globals::postBuffer);
  ether.httpPost(PSTR("/actions/lectura.js"), HOST, NULL, Globals::postBuffer, NULL);
  Serial.println(F("Lectura enviada."));
}

static char LT_Ethernet::recibirPaquetes(){
  return ether.packetReceive();
}

static char LT_Ethernet::procesarPaquetes(){
  return ether.packetLoop(ether.packetReceive());
}

static void LT_Ethernet::modoRouter(){
      char pos;
      pos = ether.packetLoop(ether.packetReceive());
      if(!pos)return;
      Serial.println((char*)Ethernet::buffer + pos);
      LT_Ethernet::routerHTTP((char*)Ethernet::buffer + pos);
}

static char* LT_Ethernet::punteroAlPaquete(){
  // return pos;
  return NULL;
}

static word LT_Ethernet::TamanioDelPaquete(){
  // return len;
	return NULL;
}

static void LT_Ethernet::actualizarDatosDesdeURI(char* uri){
  char str[16];
  if(ether.findKeyVal(uri,str, sizeof str,"myip"))ether.parseIp(myip,str);
  if(ether.findKeyVal(uri,str, sizeof str,"gwip"))ether.parseIp(gwip,str);
  if(ether.findKeyVal(uri,str, sizeof str,"hisip"))ether.parseIp(hisip,str);
  if(ether.findKeyVal(uri,str, sizeof str,"netmask"))ether.parseIp(netmask,str);
  if(ether.findKeyVal(uri,str, sizeof str,"port"))hisport=atoi(str);
}

static void LT_Ethernet::routerHTTP(char* cbuffer){
  if(strstr(cbuffer, "GET / ") != 0){
    memcpy_P(ether.tcpOffset(), pagina, sizeof pagina);
    ether.httpServerReply(sizeof pagina - 1);
  }else if(strstr(cbuffer, "GET /config?") != 0){
    LT_Ethernet::actualizarDatosDesdeURI(cbuffer+6);
    memcpy_P(ether.tcpOffset(), pagina_config, sizeof pagina_config);
    ether.httpServerReply(sizeof pagina_config - 1);
    /*
    Serial.println(F("Nueva config:"));
    ether.printIp("IP:       ", Globals::myip);
    ether.printIp("GW:       ", Globals::gwip);
    ether.printIp("SRV:      ", Globals::hisip);
    ether.printIp("DNS:      ", Globals::dnsip);
    ether.printIp("NETMASK:  ", Globals::netmask);
    Serial.println(cbuffer);
    */
    
    LT_MemoriaEEPROM::saveEthernetConfigEEPROM(myip,gwip,hisip,netmask,hisport);

    //CONFIGURA EL INDICE EN 0 (BORRAR TODOS LOS DATOS)
    Globals::indice = 0;
    LT_MemoriaEEPROM::guardarIndice();
    LT_MemoriaEEPROM::setModoConfig(false);
  }
}