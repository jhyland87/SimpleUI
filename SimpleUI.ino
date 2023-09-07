#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
//#include "TextAnchor.hpp"
//#include <ArduinoSTL.h>
//#include <cstddef>
//#include <MemoryUsage.h>

#include "SimpleUI.hpp"
#include "ColorCodes.hpp"


// START ROTARY ENCODER STUFF
#define ENCODER_DT      3     // Encoder output A (... or is it B?... idk)
#define ENCODER_CLK     4     // Encoder output B?
#define ENCODER_SW      2     // Encoder switch/button, will set the digipot to encoders val

int buttonState;                        // the current reading from the input pin
int lastButtonState             = LOW;  // the previous reading from the input pin

unsigned long lastDebounceTime  = 0;    // the last time the output pin was toggled
unsigned long debounceDelay     = 50;   // the debounce time; increase if the output flickers

volatile int encoderPos         = 0;    // Position of encoder, restricted to the ENCODER_[MAX/MIN] vals
volatile int currentStateCLK; // Input from encoders CLK pin
volatile int lastStateCLK;    // Value of the CLK in the last loop (checking for updates)
volatile bool clkClockwise;   // True if rotary is turning clockwise (this isn't really used).
// END ROTARY ENCODER STUFF



// declare size of working string buffers. Basic strlen("d hh:mm:ss") = 10
const size_t    MaxString               = 20;

// the string being displayed on the SSD1331 (initially empty)
char oldTimeString[MaxString]           = { 0 };

//Adafruit_SSD1351 oled = Adafruit_SSD1351(128, 128, &SPI, 10, 9, -1 );
Adafruit_SSD1351 oled(128, 128, &SPI, 10, 9, -1 );
//OledText Greeting(oled);

int32_t iter = 0;

uint8_t maxRows = 10;
int thisRow = 1;
char buffer[8];

SimpleUI SUI(oled);

// Display stuff
TextAnchor * TimeDisplay;

TextAnchor * Milliseconds;
TextAnchor * MillisecondsLabel;
TextAnchor * Minutes;
TextAnchor * MinutesLabel;
TextAnchor * Iterations;
TextAnchor * IterationsLabel;
TextAnchor * EncoderPos;
TextAnchor * EncoderPosLabel;

/*
// Menu Items
TextAnchor * PumpDurtion; // How long should the pump run for overall
TextAnchor * PumpDrain; // How often should the pumps drain open?
TextAnchor * DrainDuration; // How long should the drain stay open for?
*/


void setup() {
  Serial.begin(115200);
  Serial.println("In setup()");

  delay(1000);

  pinMode(ENCODER_CLK,INPUT);
  pinMode(ENCODER_DT,INPUT);
  pinMode(ENCODER_SW, INPUT);

  // Read the initial state of CLK
  lastStateCLK = digitalRead(ENCODER_CLK);

  // Call updateEncoder() when any high/low changed seen
  // on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_DT), updateEncoder, CHANGE);

 
  SUI.begin(0x95e7, 0x0000);

  
  //oled.sendCommand(0xA0);
  //oled.sendCommand(0xA0);   // 
  //oled.sendCommand(0x72); // RGB Color
  //oled.sendCommand(0xA1); // 0xA1
  //oled.sendCommand(0x0);
  

  Serial.println("Pausing for 1 sec..");
  delay(1000);
  //oled.fillScreen(BLACK);
  oled.fillRect(0, 0, 128, 128, BLACK); 
  delay(500);


  TimeDisplay   = SUI
    .newAnchor(20, 0)
    //.newAnchor(120, 0)->rightAlign(true) // Right align
    //->setColor(0xc7a9)
    ->print("<TimeDisplay>");
  //PumpDurtion   = SUI.newAnchor(0, 10)->setColor(LIGHTSTEELBLUE)->print("<PumpDurtion>");
  //PumpDrain     = SUI.newAnchor(0, 20)->setColor(LIGHTSTEELBLUE)->print("<PumpDrain>");
  //DrainDuration = SUI.newAnchor(0, 30)->setColor(LIGHTSTEELBLUE)->print("<DrainDuration>");

 
  MillisecondsLabel   = SUI.newAnchor(0, 15)
    //.newAnchor(65, 15)->rightAlign(true)
    ->setColor(DIMGRAY)->print("MS:");
  Milliseconds        = SUI.newAnchor(67, 15)->setColor(0xF800)->print("<ms>");

  MinutesLabel        = SUI.newAnchor(0, 25)
    //.newAnchor(65, 25)->rightAlign(true)
    ->setColor(DIMGRAY)->print("Min:");
  Minutes             = SUI.newAnchor(67, 25)->setColor(0x3666)->print("<min>");

  IterationsLabel     = SUI.newAnchor(0, 35)
    //.newAnchor(65, 35)->rightAlign(true)
    ->setColor(DIMGRAY)->print("Iterations:");
  Iterations          = SUI.newAnchor(67, 35)->setColor(0x4416)->print("<iter>");

  EncoderPosLabel     = SUI.newAnchor(0, 45)
    //.newAnchor(65, 45)->rightAlign(true)
    ->setColor(DIMGRAY)->setHighlightColor(MEDIUMSLATEBLUE)->print("Encoder:");
  EncoderPos          = SUI.newAnchor(67, 45)->setColor(DARKORANGE)
    //->setHighlightColor(ORANGERED)
    ->print("<enco>");

  delay(300);
  
}


void loop(){
  iter++;
  //std::cout << "lastChangeMs: " << TimeDisplay->lastChangeMs() << "; millis - lastChangeMs: " << (millis() - TimeDisplay->lastChangeMs()) << std::endl;

  
  if ( millis() - TimeDisplay->lastChangeMs() > 1000 ){
    setUptime();
  }
  
  if ( millis() - Milliseconds->lastChangeMs() > 500 ){
    Milliseconds->print(millis());
  }

  if ( millis() - Minutes->lastChangeMs() > 60000 ){
    Minutes->print(millis()/60000);
  }

  if ( millis() - Iterations->lastChangeMs() > 250 ){
    Iterations->print(iter);
  }
  
  int reading = digitalRead(ENCODER_SW);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        //std::cout << "Button clicked - Encoder: " << encoderPos << std::endl; 
        EncoderPos->highlight();
      }
    }
  }

  // set the LED:

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  
}

void setUptime() {

    unsigned long upSeconds = millis() / 1000;
    unsigned long days = upSeconds / 86400;
    upSeconds = upSeconds % 86400;
    unsigned long hours = upSeconds / 3600;
    upSeconds = upSeconds % 3600;
    unsigned long minutes = upSeconds / 60;
    upSeconds = upSeconds % 60;

    char newTimeString[ MaxString ] = { 0 };

    // construct the string representation
    sprintf(
        newTimeString,
        "%lu %02lu:%02lu:%02lu",
        days, hours, minutes, upSeconds
    );

   // std::cout << "oldTimeString: " << oldTimeString << std::endl;
    //std::cout << "newTimeString: " << newTimeString << std::endl;

    // has the time string changed since the last oled update?
    if ( strcmp( newTimeString, oldTimeString ) != 0) 
        strcpy( oldTimeString, newTimeString );
  

    TimeDisplay->print(newTimeString);
    //return newTimeString;
    
}

void updateEncoder(){


  // Read the current state of CLK
  currentStateCLK = digitalRead(ENCODER_CLK);

  //std::cout << "[updateEncoder] digitalRead(ENCODER_CLK): " << currentStateCLK << std::endl; 
  //std::cout << "[updateEncoder] digitalRead(ENCODER_DT): " << digitalRead(ENCODER_DT) << std::endl; 


  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  //if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(ENCODER_DT) != currentStateCLK) {

      encoderPos --;

      clkClockwise = false;//  ="CCW";
    } 
    else {
      // Encoder is rotating CW so increment
      encoderPos ++;

      clkClockwise = true;
    }
     //std::cout << "EncoderPos: " << encoderPos << std::endl; 

     EncoderPos->print(encoderPos);
  }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;
}
