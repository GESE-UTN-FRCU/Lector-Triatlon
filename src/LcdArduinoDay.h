#ifndef _LCD_ARDUINO_DAY_H_
#define _LCD_ARDUINO_DAY_H_

#include <ArduinoDayObject.h>

class LcdArduinoDay : public ArduinoDayObject{
  private:
    byte logo_utn[8] = {0b10101, 0b10101, 0b01110, 0b11111, 0b01110, 0b10101, 0b10101};
  public:
    void iniciarLCD();
    void iniciarLogoLCD();
    void cambiarLineaLCD(const char *linea);
}


#endif

