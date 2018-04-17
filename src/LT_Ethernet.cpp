#include <LT_Ethernet.h>

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

// Iniciar placa ethernet.
static void LT_Ethernet::iniciarModulo(){
  //ACA DEBERIA CARGAR ESTO DE MEMORIA):
  if (LT_MemoriaEEPROM::chequearModoConfig()){
    uint8_t myip[] = { 192,168,8,183 };
    uint8_t gwip[] = { 192,168,8,1 };
    uint8_t hisip[] = { 192,168,8,127 };
    uint8_t dnsip[] = { 8,8,8,8 };
    uint8_t netmask[] = { 255,255,255,0 };
    uint8_t mymac[] = { 0x74,0x69,0x69,0xAA,0x30,0x20 };
    Globals::ethernet->staticSetup(myip, gwip, dnsip, netmask);
    Globals::ethernet->parseIp(Globals::ethernet->hisip, hisip);
  }
  else{
    LT_MemoriaEEPROM::cargarDesdeEEPROM(); //ESTE NOMBRE TIENE QUE SER MAS SIGNIFICATIVO ES UNA GARCHA!
    Globals::ethernet->staticSetup(Globals::myip, Globals::gwip, Globals::dnsip, Globals::netmask); 
    Globals::ethernet->parseIp(Globals::ethernet->hisip, Globals::hisip);
  }

  delay(1000);

  Globals::ethernet->hisport = 3000;
  Globals::ethernet->registerPingCallback(gotPinged);

  // Verificar que funcione correctamente la placa Ethernet.
  if (Globals::ethernet->begin(Globals::ETHERNET_BUFFER_SIZE, Globals::mymac, Globals::PIN_ETH_SDA) == 0){
    Serial.println(F("Error de Ethernet."));
    while(1);
  }
}

static void LT_Ethernet::imprimirConfiguracion(){
  /*
  Globals::ethernet->printIp("Mi IP: ", Globals::ethernet->myip);
  Globals::ethernet->printIp("Masc. de subred: ", Globals::ethernet->netmask);
  Globals::ethernet->printIp("IP del Gateway: ", Globals::ethernet->gwip);
  Globals::ethernet->printIp("IP del DNS: ", Globals::ethernet->dnsip);
  Globals::ethernet->printIp("IP del servidor: ", Globals::ethernet->hisip);
  Serial.print("Puerto del servidor: ");
  Serial.println(Globals::ethernet->hisport);
  */
}

static bool LT_Ethernet::chequearConexion(byte *ip){

  uint32_t timer=0;
  uint8_t intentos=1;

  while(intentos<=10){
    word len = Globals::ethernet->packetReceive();
    Globals::ethernet->packetLoop(len);

    // Si recibe algo del ping, se cierra el bucle.
    if (len > 0 && Globals::ethernet->packetLoopIcmpCheckReply(ip)) break;
    
    // Si pasaron mas de 5 segundos del ultimo intento,
    // intenta nuevamente.
    if (millis() - timer >= 2000) {

      Globals::lcd->setCursor(9,1);
      Globals::lcd->print(F("("));
      Globals::lcd->print(intentos);
      Globals::lcd->print(F(")..."));
      Serial.println(F("Haciendo ping al host."));

      Globals::ethernet->clientIcmpRequest(ip);
      timer = millis();
      intentos++;
    }
  }
  if(intentos<=10)return true;
  return false;

}

const char HOST[] PROGMEM = "192.168.8.143:3000";

static void LT_Ethernet::enviarLectura(uint32_t millis, uint32_t codigo){

  Serial.println(F("Enviando lectura:"));

  sprintf(Globals::postBuffer,"m=%lu&c=%lu", millis, codigo);
  Serial.println(Globals::postBuffer);
  Globals::ethernet->httpPost(PSTR("/actions/lectura.js"), HOST, NULL, Globals::postBuffer, NULL);
  Serial.println(F("Lectura enviada."));
}

static char LT_Ethernet::recibirPaquetes(){
  return Globals::ethernet->packetReceive();
}

static char LT_Ethernet::procesarPaquetes(){
  return Globals::ethernet->packetLoop(recibirPaquetes());
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
  if(Globals::ethernet->findKeyVal(uri,str, sizeof str,"myip"))Globals::ethernet->parseIp(Globals::myip,str);
  if(Globals::ethernet->findKeyVal(uri,str, sizeof str,"gwip"))Globals::ethernet->parseIp(Globals::gwip,str);
  if(Globals::ethernet->findKeyVal(uri,str, sizeof str,"hisip"))Globals::ethernet->parseIp(Globals::hisip,str);
  if(Globals::ethernet->findKeyVal(uri,str, sizeof str,"netmask"))Globals::ethernet->parseIp(Globals::netmask,str);
  if(Globals::ethernet->findKeyVal(uri,str, sizeof str,"port"))Globals::hisport=atoi(str);
}

static void LT_Ethernet::routerHTTP(char* cbuffer){
  if(strstr(cbuffer, "GET / ") != 0){
    memcpy_P(Globals::ethernet->tcpOffset(), pagina, sizeof pagina);
    Globals::ethernet->httpServerReply(sizeof pagina - 1);
  }else if(strstr(cbuffer, "GET /config?") != 0){
    LT_Ethernet::actualizarDatosDesdeURI(cbuffer+6);
    memcpy_P(Globals::ethernet->tcpOffset(), pagina_config, sizeof pagina_config);
    Globals::ethernet->httpServerReply(sizeof pagina_config - 1);
    /*
    Serial.println(F("Nueva config:"));
    Globals::ethernet->printIp("IP:       ", Globals::myip);
    Globals::ethernet->printIp("GW:       ", Globals::gwip);
    Globals::ethernet->printIp("SRV:      ", Globals::hisip);
    Globals::ethernet->printIp("DNS:      ", Globals::dnsip);
    Globals::ethernet->printIp("NETMASK:  ", Globals::netmask);
    Serial.println(cbuffer);
    */
    
    LT_MemoriaEEPROM::guardarEnEEPROM();

    //CONFIGURA EL INDICE EN 0 (BORRAR TODOS LOS DATOS)
    Globals::indice = 0;
    LT_MemoriaEEPROM::guardarIndice();
    LT_MemoriaEEPROM::setModoConfig(false);
  }
}