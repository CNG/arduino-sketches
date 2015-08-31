#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(143, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

int pixel = random(0,142);
int color = random(0,255);

for(int i = 0; i<25; i++){
  strip.setPixelColor(pixel+i, Wheel(color));
}
for(int i = 1; i<256; i=i+10){
  strip.setBrightness(i);
  strip.show(); // Initialize all pixels to 'off'
//  delay(5);
}
for(int i = 255; i >= 0; i=i-10){
  strip.setBrightness(i);
  strip.show(); // Initialize all pixels to 'off'
//  delay(5);
}
for(int i = 0; i<25; i++){
  strip.setPixelColor(pixel+i, strip.Color(0,0,0));
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

