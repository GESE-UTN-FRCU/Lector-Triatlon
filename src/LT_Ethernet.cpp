#include <LT_Ethernet.h>

// Iniciar placa ethernet.
static void LT_Ethernet::iniciarModulo(){
	/*
	¿Usaremos configuracion de ethernet?
	Asignar la configuracion de la placa Ethernet usando
	los valores estaticos extraidos de la EEPROM.
	*/
	//Globals::ethernet->staticSetup(Globals::myip, Globals::gwip, Globals::dnsip, Globals::netmask);

	if (!Globals::ethernet->dhcpSetup())
		Serial.println("DHCP failed");

	if (!Globals::ethernet->dnsLookup("google.com"))
	    Serial.println("DNS failed");

	delay(1000);

	// Verificar que funcione correctamente la placa Ethernet.
	if (Globals::ethernet->begin(Globals::ETHERNET_BUFFER_SIZE, Globals::mymac, Globals::PIN_ETH_SDA) == 0){
		Serial.print("Error de Ethernet.");
		while(1);
	}
}

static void LT_Ethernet::imprimirConfiguracion(){
	Globals::ethernet->printIp("Mi IP: ", ether.myip);
	//Globals::ethernet->printIp("Masc. de subred: ", ether.mymask);
	Globals::ethernet->printIp("IP del Gateway: ", ether.gwip);
	Globals::ethernet->printIp("IP del DNS: ", ether.dnsip);
	Globals::ethernet->printIp("IP del servidor: ", ether.hisip);
}

static bool LT_Ethernet::chequearConexion(byte *ip){
	return LT_Ethernet::chequearConexion(ip, NULL);
}

static bool LT_Ethernet::chequearConexion(byte *ip, void (*callBack)(byte)){
  uint32_t timer=0;
  uint8_t intentos=1;

  while((recibirPaquetes() && Globals::ethernet->packetLoopIcmpCheckReply(ip)) && (intentos<=10)){
    // Si pasaron mas de 5 segundos del ultimo intento,
    // intenta nuevamente.
    if ((millis() - timer) >= 5000) {
      callBack(intentos);
      Globals::ethernet->clientIcmpRequest(ip);
      timer = millis();
      intentos++;
    }
  }
  if(intentos<=10)return true;
  return false;
}

static void LT_Ethernet::enviarJSON(char *url, JsonObject& data){
  // Se crea un stash.
  byte sd = Globals::stash.create();

  // Guarda el JSON en un string.
  String JSON_String;
  data.printTo(JSON_String);

  // Guarda el string JSON en una variable POST.
  Globals::stash.print("json_data=");
  Globals::stash.print(JSON_String);
  
  // Guarda el Stash y obtiene su tamaño.
  Globals::stash.save();
  int stash_size = Globals::stash.size();
  
  // Formatea el Stash como una peticion POST de HTTP.
  Stash::prepare(PSTR(
    "POST $S HTTP/1.0" "\r\n"
    "Content-Type: application/x-www-form-urlencoded" "\r\n"
    "Content-Length: $D" "\r\n"
    "\r\n"
    "$H"),
  url, stash_size, sd);
  
  Globals::session = Globals::ethernet->tcpSend();
}

static char LT_Ethernet::recibirPaquetes(){
	len = Globals::ethernet->packetReceive();
	return len;
}

// Funcion que se debe abrir siempre para que funcione la placa ethernet.
static char LT_Ethernet::procesarPaquetes(){
	pos = Globals::ethernet->packetLoop(recibirPaquetes());
	return pos;
}

static char LT_Ethernet::punteroAlPaquete(){
	return pos;
}

static word LT_Ethernet::TamanioDelPaquete(){
	return len;
}