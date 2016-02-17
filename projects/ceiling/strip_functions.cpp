#include "strip_functions.h"

/**
 * See if button was pushed. If so, delay and then set "pushed" back to false.
 * If we don't delay first, the interrupt might set "pushed" back to true, 
 * causing the condition to fire too many times on "one" button push.
 */
boolean checkButton() {
  if( pushed ){
    setBrightness();
    delay(250);
    pushed = false;
    lightMode++;
    return true;
  }
  return false;
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
  Fade from off to given color and back.
 */
void pulse(uint32_t c, int w) {
  for(uint16_t j=8; j<255; j++) {
    if( checkButton() ){ return; };
    strip.setBrightness(j-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(w);
  }
  delay(w*10+50);
  for(uint16_t j=255; j>8; j--) {
    if( checkButton() ){ return; };
    strip.setBrightness(j-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(w);
  }
  delay(w*10+50);
}

/*
  Rainbow Cycle Program - Equally distributed
 */
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) { // cycle all wheel colors
    if( checkButton() ){ return; };
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(
        i,
        Wheel(
          // i*256/numPixels() is like mapping i in 0...numPixels() to 0..256
          // 
          // + j & 255 ensures we change color each time we visit given pixel
          // while not running over 255
          // 
          // casting to uint32_t lets us handle strips longer than 256 pixels
          // since that would result in number larger than 32K, causing wraparound
          // to negative number
          ( (uint8_t) ( (uint32_t) i * 256 / strip.numPixels()) + j) & 255
        )
      );
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
    // kind of a hack to allow negative wait value to speed rainbow by
    // skipping colors, otherwise 0 would be fast as chip could process
    if(wait < 0 &&  j % ( -1 * wait ) != 0){
      continue; 
    }
    if( checkButton() ){ return; };
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
    if( checkButton() ){ return; };
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
