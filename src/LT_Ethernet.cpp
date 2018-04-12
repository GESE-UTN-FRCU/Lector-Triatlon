#include <LT_Ethernet.h>

// Iniciar placa ethernet.
static void LT_Ethernet::iniciarModulo(){
  // No usar esto de aca abajo por favor:
  uint8_t myip[] = { 192,168,8,184 };
  uint8_t gwip[] = { 192,168,8,1 };
  uint8_t hisip[] = { 192,168,8,143 };
  uint8_t dnsip[] = { 8,8,8,8 };
  uint8_t netmask[] = { 255,255,255,0 };
  uint8_t mymac[] = { 0x74,0x69,0x69,0xAA,0x30,0x20 };
  Globals::ethernet->staticSetup(myip, gwip, dnsip, netmask);
  delay(1000);

  Globals::ethernet->copyIp(Globals::ethernet->hisip, hisip);
  Globals::ethernet->hisport = 3000;

  Globals::ethernet->registerPingCallback(gotPinged);

  // Verificar que funcione correctamente la placa Ethernet.
  if (Globals::ethernet->begin(Globals::ETHERNET_BUFFER_SIZE, mymac, Globals::PIN_ETH_SDA) == 0){
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

static word LT_Ethernet::recibirPaquetes(){
  return Globals::ethernet->packetReceive();
}

static word LT_Ethernet::procesarPaquetes(){
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