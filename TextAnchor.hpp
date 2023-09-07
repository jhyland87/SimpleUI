#pragma once

#ifndef TEXT_ANCHOR_H
#define TEXT_ANCHOR_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
//#include <Arduino_Helpers.h>
#include <SPI.h>
//#include <ArduinoSTL.h>
//#include <cstddef>
#include "gfxfont.h"
//#include <Fonts/FreeMonoBoldOblique12pt7b.h>
//#include <Fonts/FreeSerif9pt7b.h>
//#include <Fonts/FreeMono9pt7b.h>
//#include <ArxTypeTraits.h>


#include "Settings.hpp"
//#include "TextAnchor_Templates.hpp"

// Including "settings" here just to make sharing it simpler
//constexpr unsigned int  VALUE_TYPE_NONE     = 0;  

//const size_t MAX_CHAR_LENGTH                = 60; 



/*
template <typename T_ty> struct TypeInfo { static const char * name; };
template <typename T_ty> const char * TypeInfo<T_ty>::name = "unknown";




#define TYPE_NAME(var) TypeInfo< typeof(var) >::name

// Handy macro to make defining stuff easier.
#define MAKE_TYPE_INFO(type)  template <> const char * TypeInfo<type>::name = #type;
*/


class TextAnchor {
  public: 

    TextAnchor(Adafruit_SSD1351 &GFXHandler);

    ~TextAnchor(void);
    /*
    template <class T>
      auto print(T val)
      -> typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
      //-> typename std::conditional<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
      {
        //std::cout << "Printing number: " << val << std::endl;
        
        char * _t = TYPE_NAME(val);
        std::cout << "is_integral val: " << val << "; TYPE_NAME(val): "  << _t << std::endl;

        if ( _t == "long" || _t == "long*" || _t == "unsigned long" ){
          return _printLong(val);
        }

        if ( _t == "double" || _t == "double*"  ){
          return _printFloat((double) val);
        }

        if ( _t == "float" || _t == "float*"  ){
          return _printFloat(val);
        }
        
        if ( _t == "int" || _t == "int*" ){
          return _printInt((int)val);
        }
        
      }


    template <class T>
      auto append(T val)
      -> typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
      //-> typename std::conditional<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
      {
        char * _t = TYPE_NAME(val);
        std::cout << "is_integral val: " << val << "; TYPE_NAME(val): "  << _t << std::endl;
        if ( _t == "long" || _t == "long*" || _t == "unsigned long" ){
          return _appendLong(val);
        }

        if ( _t == "double" || _t == "double*"  ){
          return _appendFloat((double) val);
        }

        if ( _t == "float" || _t == "float*"  ){
          return _appendFloat(val);
        }
        
      return  _appendInt((int)val);
      }

    template <class T>
      auto prepend(T val)
      -> typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
      //-> typename std::conditional<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
      {
        char * _t = TYPE_NAME(val);
        //std::cout << "is_integral val: " << val << "; TYPE_NAME(val): "  << _t << std::endl;
        if ( _t == "long" || _t == "long*" || _t == "unsigned long" ){
          return _prependLong(val);
        }

        if ( _t == "double" || _t == "double*"  ){
          return _prependFloat((double) val);
        }

        if ( _t == "float" || _t == "float*"  ){
          return _prependFloat(val);
        }
        
        _prependInt((int)val);
      }

      */
    
    TextAnchor * print(int intValue);
    TextAnchor * print(unsigned int intValue);
    TextAnchor * print(long intValue);
    TextAnchor * print(unsigned long intValue);
    TextAnchor * print(float intValue);
    TextAnchor * print(unsigned float intValue);
  
    // Set/update the print containers display value to a string, then display it
    TextAnchor * print(char *charValue);
    //void print(char charValue[]);
    //void print(char charValue[25]);
    
    // Set/update the print containers display value to an integer, then display it
    //void print(long intValue);
    /*
    void print( const int intValue )            { _printInt( intValue );      };
    void print( const unsigned int intValue )   { _printUInt( intValue );     };
    void print( const float intValue )          { _printDouble( intValue );   };
    void print( const double intValue )         { _printDouble( intValue );   };
    void print( const long intValue )           { _printLong( intValue );     };
    void print( const unsigned long intValue )  { _printULong( intValue );    };
    */
    TextAnchor * append( const int intValue )           { _appendInt( intValue );     };

    TextAnchor * append( const double intValue )        { _appendDouble( intValue );  };

    TextAnchor * prepend( const int intValue )          { _prependInt( intValue );    };

    TextAnchor * prepend( const double intValue )       { _prependDouble( intValue ); };

    TextAnchor * incrementPos ( int x = 0, int y = 0);
    
    TextAnchor * move( int x = 0, int y = 0 );

    TextAnchor * rightAlignAt( int x, int y ); 

    TextAnchor * setCursor( int x = 0, int y = 0 );

    TextAnchor * setColor( uint16_t fgColor );

    TextAnchor * setColor( uint16_t fgColor, uint16_t bgColor );

    TextAnchor * setColor( uint16_t fgColor, uint16_t bgColor, uint16_t fgHlColor, uint16_t bgHlColor );

    // Function to re-print whatever value/type is saved (will be useful if the
    // printed text needs to be "reloaded" for various reasons)
    TextAnchor * print( void );

    // Function to prepend some text to the value currently in the print container
    TextAnchor * prepend(char *prependTxt);
    //void prepend(char prependTxt[25]);
    //void prepend(char prependTxt[]);

    TextAnchor * append(char *appendTxt);
    //void append(const int intValue )            { _appendInt( intValue );    };

    TextAnchor * clearFromDisplay();

    TextAnchor * clear();

    TextAnchor * setFontSize( int ftSize );

    //void setFont();

    //void setFont(const GFXfont *f = NULL);

    //void setFont(const GFXfont *f = NULL);

    TextAnchor * setFontSize( int ftSize, bool reprint );

    // If this is set to true, then changing settings (font size, color, value, etc) won't automatically
    // update the display unless you call the print() function (with no parameters).
    TextAnchor * setAutoPrint( bool autoPrint );

    // Toggle highlight status
    TextAnchor * highlight();

    // Set highlight status
    TextAnchor * highlight( bool highlight );

    // Set the highlight color of just the text
    TextAnchor * setHighlightColor( uint16_t fgHlColor );

    // Set the highlight color of the text and background
    TextAnchor * setHighlightColor( uint16_t fgHlColor, uint16_t bgHlColor );

    // Calling rightAlign with no params will enable the right alignment
    TextAnchor * rightAlign( void );
    
    // Calling it with a boolean will set the right align to that specific val
    TextAnchor * rightAlign( bool rightAlign );

    uint16_t charWidth(const char ch);

    uint16_t textWidth(const char* text);

    // Retrieve the ms the last update was done
    uint32_t lastChangeMs(void);

  private:

    Adafruit_SSD1351 * GFXHandler;
    GFXfont *gfxFont; 

    uint16_t _fontCharWidth       = 6;            // How many pixels wide is 1 character
    uint16_t _fontCharHeight; // How many pixels high is 1 character

    // When set to true, calling any of the set(FontSize|Color|Location) will automatically
    // reprint the text area with the new settings.
    bool _autoReprint             = true;
    bool _highlightStatus         = false;
    bool _rightAlign              = false;
    bool _allowInterruptPauses    = true;               // Will allow noInterrupts() and interrupts() to be
                                                        // called from the print() function. Without this,
                                                        // sometimes other print() executions can be called
                                                        // between the cursor placement and actual print, 
                                                        // which places values in the wrong locations.

    int _x                        = NULL;
    int _y                        = NULL;
    uint32_t _lastChangeMs        = millis();

    int _textSize                 = FONT_DEFAULT_SIZE;

    uint16_t _fgHlColor           = NULL;
    uint16_t _bgHlColor           = NULL;

    uint16_t  _fgColor            = NULL; 
    uint16_t  _bgColor            = NULL;

    // Current char/string being displayed (or to be displayed)
    char _printerVal[ MAX_CHAR_LENGTH ]; 

    // The previous string val, used when overriding it with shorter text
    // NOTE:  This may not be needed. Should be able to check for the 
    //        value/length of the currently displayed text using whats
    //        stored in _printerVal.
    char _printerValPrev[ MAX_CHAR_LENGTH ]; 


    void _printInt( int intValue );
    void _printUInt( unsigned int intValue );
    void _printLong( long intValue );
    void _printULong( unsigned long intValue );
    void _printFloat( float intValue );
    void _printDouble( double intValue );

    void _appendInt( int intValue );
    void _appendDouble( double intValue );

    void _prependInt( int intValue );
    void _prependDouble( double intValue );
};

#endif // TEXT_ANCHOR_H