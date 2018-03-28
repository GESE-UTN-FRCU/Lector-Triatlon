#ifndef _LT_LCD_H_
#define _LT_LCD_H_

#include <Arduino.h>
#include <LT_Globals.h>

class LT_LCD{
  private:
    static byte logo_utn[8] = {0b10101, 0b10101, 0b01110, 0b11111, 0b01110, 0b10101, 0b10101};
  public:
    static void iniciarModulo();
    static void iniciarLogoLCD();
    static void cambiarLineaLCD(const char *linea);
};

#endif