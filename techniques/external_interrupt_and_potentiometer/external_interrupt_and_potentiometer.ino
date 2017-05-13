/*
  This is an example of an external interrupt that causes NeoPixel brightness to
  be set by reading from a potentiometer.

  The program begins by setting brightness based on the pot, but then does not
  read the pot again until a button is pushed.

  The original rainbow() and Wheel() functions are from Adafruit's NeoPixel
  example sketch at https://github.com/adafruit/Adafruit_NeoPixel/

  Tested on Adafruit FLORA (https://www.adafruit.com/products/659)
*/


#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define POT_PIN 9 // using 1K potentiometer

Adafruit_NeoPixel strip = Adafruit_NeoPixel(64, LED_PIN, NEO_GRB + NEO_KHZ800);

boolean pushed = false;


void setup() {
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

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


// Instead of changing brightness directly within interrupt, we'll just toggle a
// variable that we'll check for later. This prevents any weirdness due to 
// variable volatility.
void buttonPushed() {
  pushed = true;
}


// Set strip brightness based on potentiometer value
void setBrightness() {
  // convert analogRead's range of 0 to 1023 to brightness range 0 to 255
  uint8_t brightness = map(analogRead(POT_PIN), 0, 1023, 0, 255);
  strip.setBrightness(brightness);
}


void loop() {
  rainbow(10);
}


// cycle each pixel through 256 colors
void rainbow(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {

      // detect if interrupt occurred since last pixel
      // (this check could go out one loop, but it's still fast enough)
      if(pushed){
        setBrightness();
        pushed = false; // reset interrupt detection
      }

      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
