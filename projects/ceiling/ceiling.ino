#include <Adafruit_NeoPixel.h>


/*
  TODO (this includes nonsettings)
  Hardware and connection dependent settings
 */

uint8_t lightMode = 2;

#include "strip_functions.h"
uint16_t _num_led = 693; // Number of pixels in strand
uint8_t _pin_led = 6; // NeoPixel LED strand is connected to this pin
uint8_t _pin_pot = 9; // using 1K potentiometer

#include "sound_reactive.h"
uint8_t _pin_mic = A10; // Microphone is attached to this analog pin
extern int vol;


/* Initialize variables */

Adafruit_NeoPixel
  strip = Adafruit_NeoPixel(_num_led, _pin_led, NEO_GRB + NEO_KHZ800);
boolean pushed = false;

//int lightMode = 0; // how many times the button has been pressed

void setup() {

  /* Interrupt setup */

    /*
      "FLORA supports external interrupts on D0 (INT2), D1(INT3), D2(INT1), and 
      D3(INT0). You would attach your button between, say, D0 and GND."
      FLORA's D0 is labeled "RX" (https://learn.adafruit.com/assets/2845)
      and corresponds to Arduino interrupt 2.
      * https://www.arduino.cc/en/Reference/PinMode
      * https://www.arduino.cc/en/Reference/attachInterrupt
      * https://forums.adafruit.com/viewtopic.php?f=25&t=57650:
     */
    pinMode(0, INPUT_PULLUP); // configure pin to behave as input
    // when button is pushed, run buttonPushed()
    attachInterrupt(2, buttonPushed, FALLING); // listen for high to low

    // from Amplitie code, not sure if needed or what it does
    //pinMode(_pin_led, OUTPUT);
  
  /* sound_reactive */

    memset((void*) vol, 0, sizeof(vol));

    /*
      This is only needed on 5V Arduinos (Uno, Leonardo, etc.). Connect 3.3V to 
      mic AND TO AREF ON ARDUINO and enable this line.  Audio samples are 
      'cleaner' at 3.3V. COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.): 
     */
    // analogReference(EXTERNAL);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}




void loop() {

  // detect if interrupt occurred since last pixel
  // (this check could go out one loop, but it's still fast enough)
  if(pushed){
    setBrightness();
    pushed = false; // reset interrupt detection
  }         

  if (lightMode == 0) {
    colorWipe(strip.Color(0, 0, 0), 0);
    delay(20);
  } else if (lightMode == 1) {
    
    rainbowSingle(3);

  } else if (lightMode == 2) {
    //rainbow(1);
    rainbowCycle(1);
  } else {
    int counter = 0;
    while (counter < 100) {
      volMeter(lightMode);
      counter++;
    }
  }

}

