#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  //strip.setBrightness(30);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  uint16_t dot, pixel, color;
  color=random(0,255);
  for(dot=0; dot<strip.numPixels()/2; dot++) {
    
    strip.setPixelColor(0, Wheel(color));
    strip.setPixelColor(1, Wheel(color));
    strip.show();
      color=color+2&255;
    for(pixel=3; pixel<strip.numPixels()-dot*2; pixel++) {

      strip.setPixelColor(pixel-3, strip.Color(0,0,0));
      strip.setPixelColor(pixel-2, strip.Color(0,0,0));
      strip.setPixelColor(pixel-1, Wheel(color));
      strip.setPixelColor(pixel, Wheel(color+1&255));
      strip.show();
      delay(round(15*(61-(dot+1))/61));
      
    }
    
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
