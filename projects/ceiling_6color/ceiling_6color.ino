#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(143, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
}

void loop() {
  rainbow(500);
}

void rainbow(uint8_t wait) {

  uint32_t black = strip.Color(0,0,0);
  uint32_t colors[6];
  colors[0] = strip.Color(255,0,0);
  colors[1] = strip.Color(128,128,0);
  colors[2] = strip.Color(0,255,0);
  colors[3] = strip.Color(0,128,128);
  colors[4] = strip.Color(0,0,255);
  colors[5] = strip.Color(128,0,128);

  for(uint8_t i=0; i<6; i++) {
    for(uint16_t j=0; j<strip.numPixels(); j++) {
      strip.setPixelColor(j, colors[i]);
    }
    strip.show();
    delay(wait);
//    for(uint16_t j=0; j<strip.numPixels(); j++) {
//      strip.setPixelColor(j, black);
//    }
//    strip.show();
//    delay(wait*9);
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

