#pragma once

#include "TextAnchor.hpp"



MAKE_TYPE_INFO( int )
MAKE_TYPE_INFO( int* )
MAKE_TYPE_INFO( float )
MAKE_TYPE_INFO( float* )
MAKE_TYPE_INFO( long )
MAKE_TYPE_INFO( unsigned long )
MAKE_TYPE_INFO( long* )
MAKE_TYPE_INFO( short* )
MAKE_TYPE_INFO( short )
MAKE_TYPE_INFO( double )
MAKE_TYPE_INFO( double* )
MAKE_TYPE_INFO( char )
MAKE_TYPE_INFO( char* )
MAKE_TYPE_INFO( char& )
//MAKE_TYPE_INFO( String )
//MAKE_TYPE_INFO( char[5] )
//MAKE_TYPE_INFO( char(*)[5] )



/*
https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.cpp
*/
TextAnchor::TextAnchor(Adafruit_SSD1351 &GFXHandler) : GFXHandler (&GFXHandler) {}

// If user were to set/change the printer to a CHAR...
TextAnchor * TextAnchor::print( char *charValue ) {
  //std::cout << "You provided the char: " << charValue << std::endl;

  //if ( strcmp( _printerVal, _printerValPrev ) != 0 )  _lastChangeMs = millis();
  

  //_printerVal = charValue;
  strcpy(_printerVal, charValue);

  print();

  return this;
}

TextAnchor::~TextAnchor(void) {
  std::cout << "Were in ~TextAnchor" << std::endl;
  free(this);
}

TextAnchor * TextAnchor::setAutoPrint( bool autoPrint ){
  _autoReprint = autoPrint;
  return this;
}

TextAnchor * TextAnchor::setCursor( int x, int y ) {
  _x = x;
  _y = y;

  print();

  return this;
}

// A specific x/y location is provided, and the test will always end there
// whenever updated. 
TextAnchor * TextAnchor::rightAlign( void ){
  return rightAlign( true );
}

TextAnchor * TextAnchor::rightAlign( bool rightAlign ){
  if ( rightAlign == _rightAlign )
    return this;
  
  _rightAlign = rightAlign;
  print();

  return this;
}

// This would print the stored value in the specified area, but for this forum
// post, I'm just outputting the value to serial
TextAnchor * TextAnchor::print( void ){
  // width, height, getRotation, getCursorX, getCursorY

  /*
  std::cout << "width: " << GFXHandler->width() << std::endl;
  std::cout << "height: " << GFXHandler->height() << std::endl;
  std::cout << "getRotation: " << GFXHandler->getRotation() << std::endl;
  std::cout << "getCursorX: " << GFXHandler->getCursorX() << std::endl;
  std::cout << "getCursorY: " << GFXHandler->getCursorY() << std::endl;
  */


  
  //std::cout << _printerVal << " has charWidth: " 
    //<< GFXHandler->charWidth(_printerVal) << " and textWidth: " 
    //<< GFXHandler->textWidth(_printerVal) << std::endl;


  uint16_t  colFg = _fgColor, 
            colBg = _bgColor;

  // If highlight is true, then set the fg/bg colors to the highlight colors
  if ( _highlightStatus == true ) {
    // If no fg/bg highlight color is set, then just swap the values
    if ( _fgHlColor == NULL && _bgHlColor == NULL ){
      colFg = _bgColor;
      colBg = _fgColor;
    }
    // But if only one of the fg or bg highlight color is set, then use that
    // color only for that fg/bg (and leave the other as-is)
    else {
      if ( _fgHlColor != NULL ) colFg = _fgHlColor;

      if ( _bgHlColor != NULL ) colBg = _bgHlColor;
    }
  }

  int diff = 0;

  // If the printed text isn't the same as the text printed last time, then check the difference
  // in length (to override the text that would otherwise show from the side), and update the 
  // _printerValPrev
  if ( strcmp( _printerVal, _printerValPrev ) != 0 ) {
    //std::cout << "\t_printerVal is different than _printerValPrev: "  << std::endl;

    if ( strlen(_printerValPrev) > strlen( _printerVal ) ){
      diff = strlen(_printerValPrev) - strlen(_printerVal );

      //std::cout << "\tdiff: " << diff  << std::endl;
    }
  }

  // Pause interrupts while we set the font details and placement. This prevents other
  // updates that may be triggered via an interrupt from unintentionally injecting
  // print values into this location. For example, this happens if a text anchor is 
  // updated in an interrupt function triggered by an encoder (if the encoder is spun
  // too quickly). 
  if ( _allowInterruptPauses == true ) noInterrupts();

  //std::cout << "getBuffer: " << GFXHandler->getBuffer() << std::endl;
  //GFXHandler.setRotation(1);  
  GFXHandler->setTextSize( _textSize );
  GFXHandler->setFont( NULL );

  //int realX = 0;

  // If right align is enabled, then set X to be n characters to left of _x,
  // where n = length of _printerVal
  if ( _rightAlign == true ){
    GFXHandler->setCursor( _x - (strlen(_printerVal) + diff) * _fontCharWidth, _y );
  }
  else {
    GFXHandler->setCursor( _x, _y );
  }
  
  GFXHandler->setTextColor( colFg, colBg );

  /*
  std::cout << "Printing: " << _printerVal  << std::endl;
  std::cout << "\t_x: "       << _x           << std::endl;
  std::cout << "\t_y: "       << _y           << std::endl;
  std::cout << "\t_fgColor: " << _fgColor     << std::endl;
  std::cout << "\t_bgColor: " << _bgColor     << std::endl;
  std::cout << "\t_printerValPrev: " << _printerValPrev     << std::endl;
  */  

  if ( diff > 0  && _rightAlign == true ){
    for ( int i = 0; diff > i; i++ ){
      //std::cout << "\tAdding prefix space #" << i  << std::endl;
      GFXHandler->print(" ");
    }
  }

  GFXHandler->print( _printerVal );

  if ( diff > 0  && _rightAlign == false ){
    for ( int i = 0; diff > i; i++ ){
      //std::cout << "\tAdding suffix space #" << i  << std::endl;
      GFXHandler->print(" ");
    }
  }

  // If the printed text isn't the same as the text printed last time, then check the difference
  // in length (to override the text that would otherwise show from the side), and update the 
  // _printerValPrev
  if ( strcmp( _printerVal, _printerValPrev ) != 0 ) {
    _lastChangeMs = millis();

    //std::cout << "\t_printerVal is different than _printerValPrev: "  << std::endl;

    /*
    if ( strlen(_printerValPrev) > strlen( _printerVal ) ){
      int diff = strlen(_printerValPrev) - strlen(_printerVal );

      //std::cout << "\tdiff: " << diff  << std::endl;

      for ( int i = 0; diff > i; i++ ){
        //std::cout << "\tAdding space #" << i  << std::endl;
        GFXHandler->print(" ");
      }
    }
    */
    strcpy(_printerValPrev, _printerVal);
  }

  if ( _allowInterruptPauses == true ) interrupts();

  return this;
}

// User wants to prepend (or append, in a diff function) a string
// to the beginning of the char value..
TextAnchor * TextAnchor::prepend( char *prependTxt ){
  if ( strlen( _printerVal ) == 0 )
  {
    // Nothing to append anything to, just switch to a regular print
    return print( prependTxt );

    //return ;
  }

  char newCharValue[ strlen(prependTxt) + strlen(_printerVal) +1 ];  // add +1 for end?...
  //char newCharValue[ MAX_CHAR_LENGTH ];
  strcpy( newCharValue, prependTxt );
  //strncpy( newCharValue, prependTxt, MAX_CHAR_LENGTH );
  strcat( newCharValue, _printerVal );

  // Print the new char (with the prefix prepended)
  print( newCharValue );

  return this;
}

TextAnchor * TextAnchor::append( char *appendTxt ){
  if ( strlen( _printerVal ) == 0 )
  {
    // Nothing to append anything to, just switch to a regular print
   return print( appendTxt );
  }

  char newCharValue[ strlen(appendTxt) + strlen(_printerVal) +1 ];  // add +1 for end?...
  //char newCharValue[ MAX_CHAR_LENGTH ];
  strcpy( newCharValue, _printerVal );
  //strcpy( newCharValue, prependTxt );
  //strncpy( newCharValue, prependTxt, MAX_CHAR_LENGTH );
  strcat( newCharValue, appendTxt );

  // Print the new char (with the prefix prepended)
  print( newCharValue );

  return this;
}

TextAnchor * TextAnchor::clear(){
  return this;
}

TextAnchor * TextAnchor::clearFromDisplay(){
  GFXHandler->setTextSize( _textSize );
  //GFXHandler->setFont( NULL );
  GFXHandler->setTextColor( _bgColor, _bgColor );
  GFXHandler->setCursor( _x, _y );
  GFXHandler->print(_printerVal);
  GFXHandler->setTextColor( _fgColor, _bgColor );

  return this;
}

TextAnchor * TextAnchor::incrementPos ( int x, int y ){
  return move( _x+x, _y+y);
}

// Move a text box to a new location. This will involve clearing the current location (covering it with 
// the background color), moving the cursor to the new location, and printing the content.
TextAnchor * TextAnchor::move( int x, int y ){
  clearFromDisplay();
  setCursor( x, y );
  print();

  return this;
}
/*
void setFont(const GFXfont *f = NULL);
  GFXHandler->setFont(GFXfont);
  //GFXHandler->setFont(&FreeMonoBoldOblique12pt7b);
}
*/

TextAnchor * TextAnchor::setFontSize( int ftSize ){
  _textSize = ftSize;

  print();

  return this;
}

TextAnchor * TextAnchor::setFontSize( int ftSize, bool reprint ){
  _textSize = ftSize;

  print();

  return this;
}



TextAnchor * TextAnchor::setColor( uint16_t fgColor ){
  _fgColor = fgColor;

  print();

  return this;
}

TextAnchor * TextAnchor::setColor( uint16_t fgColor, uint16_t bgColor ){
  _fgColor = fgColor;
  _bgColor = bgColor;

  //std::cout << "Inside: TextAnchor::setColor" << std::endl;
  //std::cout << "\t_fgColor: " << _fgColor     << std::endl;
  //std::cout << "\t_bgColor: " << _bgColor     << std::endl;

   print();

  return this;
}

TextAnchor * TextAnchor::setColor( uint16_t fgColor, uint16_t bgColor, uint16_t fgHlColor, uint16_t bgHlColor ){
  return this;
}

TextAnchor * TextAnchor::setHighlightColor( uint16_t fgHlColor ){
  _fgHlColor = fgHlColor;

  if ( _highlightStatus == true ) print();

  return this;
}

TextAnchor * TextAnchor::setHighlightColor( uint16_t fgHlColor, uint16_t bgHlColor ){
  _fgHlColor = fgHlColor;
  _bgHlColor = bgHlColor;

  //std::cout << "Inside: TextAnchor::setColor" << std::endl;
  //std::cout << "\t_fgColor: " << _fgColor     << std::endl;
  //std::cout << "\t_bgColor: " << _bgColor     << std::endl;

  if ( _highlightStatus == true ) print();

  return this;
}

TextAnchor * TextAnchor::highlight( void ){
  _highlightStatus = !_highlightStatus;

  std::cout << "In highlight - highlight:" << _highlightStatus << std::endl;

  
  /*
  if ( _highlightStatus == true ){
    setColor( _bgColor, _fgColor );
  }
  else {
    setColor( _fgColor, _bgColor );
  }
  */

  return print();

  //return this;
}

// Set highlight status
TextAnchor * TextAnchor::highlight( bool highlight ){
  _highlightStatus = highlight;
  
  if ( _highlightStatus == true ){
    setColor( _bgColor, _fgColor );
  }
  else {
    setColor( _fgColor, _bgColor );
  }

  print();
  
  return this;
}

uint32_t TextAnchor::lastChangeMs(void){
  return _lastChangeMs;
}

void TextAnchor::_appendInt( const int intValue ){
  char cstr[MAX_CHAR_LENGTH];

  itoa( intValue, cstr, 10 );

  append( cstr );
}

void TextAnchor::_prependInt( const int intValue ){
  char cstr[MAX_CHAR_LENGTH];

  itoa( intValue, cstr, 10 );

  prepend( cstr );
}

void TextAnchor::_appendDouble( const double intValue ){
  char cstr[MAX_CHAR_LENGTH];

  dtostrf(intValue, 2, 2, cstr);

  append( cstr );
}

void TextAnchor::_prependDouble( const double intValue ){
  char cstr[MAX_CHAR_LENGTH];

  dtostrf(intValue, 2, 2, cstr);

  prepend( cstr );
}

void TextAnchor::_printInt( const int intValue ){
  //std::cout << "The value " << intValue << " is a REGULAR int" << std::endl;

  char cstr[MAX_CHAR_LENGTH];

  itoa( intValue, cstr, 10 );

   print(cstr);
}

void TextAnchor::_printUInt( const unsigned int intValue ){
  //std::cout << "The value " << intValue << " is an UNSIGNED REGULAR int" << std::endl;

  char cstr[MAX_CHAR_LENGTH];

  itoa( intValue, cstr, 10 );

  print(cstr);
  //print(cstr);
}

void TextAnchor::_printLong( const long intValue ){
  //std::cout << "The value " << intValue << " is a LONG int" << std::endl;

  char cstr[MAX_CHAR_LENGTH];

  ltoa( intValue, cstr, 10 );

  if ( _autoReprint == true ) print(cstr);
}

void TextAnchor::_printULong( const unsigned long intValue ){
  std::cout << "The value " << intValue << " is a UNSIGNED LONG int" << std::endl;

  char cstr[MAX_CHAR_LENGTH];
  
  ltoa( intValue, cstr, 10 );

  print(cstr);
}

void TextAnchor::_printFloat( const float intValue ){
  //std::cout << "The value " << intValue << " is a FLOAT int" <<  std::endl;

  char cstr[MAX_CHAR_LENGTH];

  dtostrf(intValue, 2, 2, cstr);

  print(cstr);
}

void TextAnchor::_printDouble( const double intValue ){
  std::cout << "The value " << intValue << " is a DOUBLE int" <<  std::endl;

  char cstr[MAX_CHAR_LENGTH];

  dtostrf(intValue, 2, 2, cstr);

  print(cstr);
}

uint16_t TextAnchor::charWidth(const char ch)
{
  char onecharstring[2] = {ch, 0};
  int16_t x1, y1;
  uint16_t w, h;
  GFXHandler->getTextBounds(onecharstring, 0, 0, &x1, &y1, &w, &h);
  return w;
}

uint16_t TextAnchor::textWidth(const char* text)
{
  int16_t x1, y1;
  uint16_t w, h;
  GFXHandler->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  return w;
}

/*
// User provides a new NUMERICAL value, store it as a char
void  TextAnchor::print( int intValue )
{
  //typeof( intValue ) MyType1;
  std::cout << "printing int :" << intValue <<  std::endl;

  //std::cout << "MyType1:" <<  std::endl;
  //this->TypeOf(intValue);

  //typeof( 12345 ) an_int;

  //typedef typeof( intValue ) MyType2;

  //std::cout << "MyType2:" << MyType2 <<  std::endl;
  
  char cstr[MAX_CHAR_LENGTH];
  ltoa( intValue, cstr, 10 );

  print(cstr);
}
*/
/*
// User provides a new NUMERICAL value, store it as a char
void  TextAnchor::print( float intValue )
{
  //std::cout << "Type of intValue::" << typeid(intValue).name() <<  std::endl;
  std::cout << "Printing a DOUBLE:" << intValue <<  std::endl;
  
  char cstr[MAX_CHAR_LENGTH];
  //ltoa( intValue, cstr, 10 );
  // dtostrf(float_value, min_width, num_digits_after_decimal, where_to_store_string)

  dtostrf(intValue, 2, 2, cstr);

  print(cstr);
  
}
*/



/*
char* TextAnchor::TypeOf( const double& )
{
  static const char type[] = "double";  
  return type; 
}

char* TextAnchor::TypeOf( const double* ) 
{ 
  static const char type[] = "double*";  
  return type; 
}

char* TextAnchor::TypeOf( const float& )
{ 
  static const char type[] = "float";   
  return type; 
}

char* TextAnchor::TypeOf( const float* )
{ 
  static const char type[] = "float*";   
  return type; 
}

char* TextAnchor::TypeOf(const int&)
{ 
  static const char type[] = "int";
  return type; 
}

char* TextAnchor::TypeOf(const int*)
{ 
  static const char type[] = "int*";
  return type; 
}
*/

