#pragma once

#include "SimpleUI.hpp"

// Type-specific implementations.


SimpleUI::SimpleUI(Adafruit_SSD1351 &GFXHandler) : GFXHandler (&GFXHandler) {}


TextAnchor * SimpleUI::newAnchor( int x, int y ){
  std::cout << "Inside  SimpleUI::newAnchor\n\tX: " << x << "\n\tY: " << y << std::endl;

  TextAnchor * _anchor = new TextAnchor( * GFXHandler );
  _anchor->setCursor( x, y );
  _anchor->setFontSize( _textSize );

  if ( _defFgColor != NULL && _defBgColor != NULL  ){
    _anchor->setColor( _defFgColor, _defBgColor );
  }
  else if ( _defFgColor != NULL  ){
    _anchor->setColor( _defFgColor );
  }
  
  _anchor->setAutoPrint( true );
  _anchor->print();

  return _anchor;
}


/*
void SimpleUI::whatever(char str){
  std::cout << "whatever - str was a string: " << str << std::endl;
}
*/

void SimpleUI::begin(){
  GFXHandler->begin();
  GFXHandler->setFont();
  GFXHandler->fillScreen(_defBgColor);
  GFXHandler->setTextColor(_defFgColor);
  //GFXHandler.setRotation(1);  
  GFXHandler->setTextSize(1);
}

void SimpleUI::begin( uint16_t fgColor , uint16_t bgColor ){
  _defFgColor = fgColor;
  _defBgColor = bgColor;

  _begin();
}

void SimpleUI::begin( uint16_t fgColor ){
  _defFgColor = fgColor;

  _begin();
}

void SimpleUI::_begin(){
  GFXHandler->begin();
  GFXHandler->setFont();
  GFXHandler->fillScreen(_defBgColor);
  GFXHandler->setTextColor(_defFgColor);
  //GFXHandler.setRotation(1);  
  GFXHandler->setTextSize(1);
}

void SimpleUI::setColor( uint16_t fgColor ){
  std::cout << "Inside: SimpleUI::setColor(fg)" << std::endl;
  std::cout << "\tfgColor: " << fgColor     << std::endl;
  
  _defFgColor = fgColor;
}

void SimpleUI::setColor( uint16_t fgColor, uint16_t bgColor ){
  std::cout << "Inside: SimpleUI::setColor(fg, bg)" << std::endl;
  std::cout << "\tfgColor: " << fgColor     << std::endl;
  std::cout << "\tbgColor: " << bgColor     << std::endl;

  _defFgColor = fgColor;
  _defBgColor = bgColor;
}
/*
void SimpleUI::print( char val ){
  
}
*/