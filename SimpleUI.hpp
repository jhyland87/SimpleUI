#pragma once

#ifndef SIMPLE_UI_H
#define SIMPLE_UI_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
//#include <Arduino_Helpers.h>
#include <SPI.h>
//#include <ArduinoSTL.h>
//#include <cstddef>
#include "Settings.hpp"
#include "TextAnchor.hpp"


class SimpleUI {
  public: 

    SimpleUI(Adafruit_SSD1351 &GFXHandler);

    void begin();
    void begin( uint16_t fgColor );
    void begin( uint16_t fgColor, uint16_t bgColor );

    void setColor( uint16_t fgColor );
    void setColor( uint16_t fgColor, uint16_t bgColor );


    TextAnchor * newAnchor( int x, int y );

   // void whatever(char str);

    //template<class T> void whatever(T param) { std::cout << "param: " << param << "; TYPE_NAME(): "  << TYPE_NAME(param) << std::endl; }

 // void print( char val );

  private:

    Adafruit_SSD1351 * GFXHandler;

    void _begin();

    //int _x, _y;

    // DEFAULT fg/bg colors (can be changed on a TextArea basis);
    uint16_t _defFgColor = 0x95e7;
    uint16_t _defBgColor = 0x0000;

    int _textSize = FONT_DEFAULT_SIZE;

};



#endif // SIMPLE_UI_H