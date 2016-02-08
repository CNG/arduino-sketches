#include "strip_functions.h"

/*
  Instead of changing brightness directly within interrupt, we'll just toggle a
  variable that we'll check for later. This prevents any weirdness due to 
  variable volatility.
 */
void buttonPushed() {
  pushed = true;
  lightMode++;
  if (lightMode == 6) lightMode = 0;
}

/*
  Set strip brightness based on potentiometer value
 */
void setBrightness() {
  // convert analogRead's range of 0 to 1023 to brightness range 0 to 255
  uint8_t brightness = map(analogRead(_pin_pot), 0, 1023, 0, 255);
  strip.setBrightness(brightness);
}

/*
  Input a value 0 to 255 to get a color value.
  The colors are a transition r - g - b - back to r.
 */
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

/*
  Rainbow Cycle Program - Equally distributed
 */
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) { // cycle all wheel colors
    for(i=0; i< strip.numPixels(); i++) {

      // detect if interrupt occurred since last pixel
      // (this check could go out one loop, but it's still fast enough)
      if(pushed){
        setBrightness();
        pushed = false; // reset interrupt detection
      }

      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    if ( wait ) {
      strip.show();
      delay(wait);
    }
  }
  if ( wait == 0 ) {
    strip.show();
  }
}

//Rainbow Program
void rainbowSingle(int wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) {
    if(wait < 0 &&  j % ( -1 * wait ) != 0){
      continue; 
    }
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(j));
    }
    strip.show();
    if(wait > 0){
      delay(wait);
    }
  }
}

//Rainbow Program
void rainbow(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
