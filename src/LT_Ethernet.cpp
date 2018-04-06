#include <LT_Ethernet.h>

// Iniciar placa ethernet.
static void LT_Ethernet::iniciarModulo(){
  // No usar esto de aca abajo por favor:
  uint8_t myip[] = { 192,168,8,184 };
  uint8_t gwip[] = { 192,168,8,1 };
  uint8_t hisip[] = { 192,168,8,158 };
  uint8_t dnsip[] = { 8,8,8,8 };
  uint8_t netmask[] = { 255,255,255,0 };
  uint8_t mymac[] = { 0x74,0x69,0x69,0xAA,0x30,0x20 };
  Globals::ethernet->staticSetup(myip, gwip, dnsip, netmask);
  delay(1000);

  Globals::ethernet->copyIp(Globals::ethernet->hisip, hisip);
  Globals::ethernet->hisport = 4000;
  
  /* Usar esto de aca abajo por favor:
  if (!Globals::ethernet->dhcpSetup())
    Serial.println("DHCP failed");
  if (!Globals::ethernet->dnsLookup("gSADoogle.com"))
      Serial.println("DNS failed");
  */

  Globals::ethernet->registerPingCallback(gotPinged);

  // Verificar que funcione correctamente la placa Ethernet.
  if (Globals::ethernet->begin(Globals::ETHERNET_BUFFER_SIZE, mymac, Globals::PIN_ETH_SDA) == 0){
    Serial.println("Error de Ethernet.");
    while(1);
  }
}

static void LT_Ethernet::imprimirConfiguracion(){
	Globals::ethernet->printIp("Mi IP: ", Globals::ethernet->myip);
	Globals::ethernet->printIp("Masc. de subred: ", Globals::ethernet->netmask);
	Globals::ethernet->printIp("IP del Gateway: ", Globals::ethernet->gwip);
	Globals::ethernet->printIp("IP del DNS: ", Globals::ethernet->dnsip);
  Globals::ethernet->printIp("IP del servidor: ", Globals::ethernet->hisip);
	Serial.print("Puerto del servidor: ");
  Serial.println(Globals::ethernet->hisport);
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
      Serial.println(F("Haciendo ping..."));

      Globals::ethernet->clientIcmpRequest(ip);
      timer = millis();
      intentos++;
    }
  }
  if(intentos<=10)return true;
  return false;

}

static bool chequearConexion(byte *ip,void (*callBack)(byte)){
  uint32_t timer=0;
  uint8_t intentos=1;

  while(intentos<=10){
    word len = Globals::ethernet->packetReceive();
    Globals::ethernet->packetLoop(len);

    // Si recibe algo del ping, se cierra el bucle.
    if (len > 0 && Globals::ethernet->packetLoopIcmpCheckReply(ip)) break;
    
    // Si pasaron mas de 5 segundos del ultimo intento,
    // intenta nuevamente.
    if (millis() - timer >= 5000) {
      callBack(intentos);
      Globals::ethernet->clientIcmpRequest(ip);
      timer = millis();
      intentos++;
    }
  }
  if(intentos<=10)return true;
  return false;
}

static void LT_Ethernet::enviarAlgo(){
  char session;
  Stash stash;
  Serial.println("Sending tweet...");
  byte sd = stash.create(); 
  const char tweet[] = "@solarkennedy the test Twitter sketch works!";
  stash.print("token=");
  stash.print("asdasd");
  stash.print("&status=");
  stash.println(tweet);
  stash.save();
  int stash_size = stash.size();  
  // Compose the http POST request, taking the headers below and appending
  // previously created stash in the sd holder.
  Stash::prepare(PSTR("POST http://$F/update HTTP/1.0" "\r\n"
    "Host: $F" "\r\n"
    "Content-Length: $D" "\r\n"
    "\r\n"
    "$H"),
  "dsad", "dasdasd", stash_size, sd);  
  // send the packet - this also releases all stash buffers once done
  // Save the session ID so we can watch for it in the main loop.
  session = Globals::ethernet->tcpSend();  
  Globals::ethernet->tcpSend();
}

static void LT_Ethernet::enviarJSON(char *method, char *url, JsonObject& data){
  // Se crea un stash.
  Stash stash;
  byte sd = stash.create();

  // Guarda el JSON en un string.
  String JSON_String;
  data.printTo(JSON_String);

  // Guarda el string JSON en una variable POST.
  stash.print(JSON_String);
  
  // Guarda el Stash y obtiene su tamaño.
  stash.save();
  int stash_size = stash.size();
  
  // Formatea el Stash como una peticion POST de HTTP.
  Stash::prepare(PSTR(
    "$S $S HTTP/1.1" "\r\n"
    "Content-Type: application/json" "\r\n"
    "Content-Length: $D" "\r\n"
    "\r\n"
    "$H"),
  method, url, stash_size, sd);

  // Envia el stash.
  //Globals::session = Globals::ethernet->tcpSend();
  Globals::ethernet->tcpSend();
}

static word LT_Ethernet::recibirPaquetes(){
	//len = Globals::ethernet->packetReceive();
	//return len;
  return Globals::ethernet->packetReceive();
}

// Funcion que se debe abrir siempre para que funcione la placa ethernet.
static word LT_Ethernet::procesarPaquetes(){
	//pos = Globals::ethernet->packetLoop(recibirPaquetes());
	//return pos;
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