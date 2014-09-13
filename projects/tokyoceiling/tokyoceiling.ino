#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(480, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  // Serial.begin(9600); while (! Serial);
}

void loop() {

  strip.setBrightness(100);

  uint8_t q, r, s, numColors, currentColor;
  uint16_t spacing, spread;

  numColors = 16; // [1,256]
  spacing = 120;
  spread = 5;

  boolean reverse1, reverse2 = true; // state, not setting
  float colorSep = 255 / ( numColors - 1 );
  q = s = 0; // state
  uint8_t numSpreads = spacing / 20;
  while(true){
    r = q % numColors;
    s = q % numSpreads;

    if (r == 0) { reverse1 = !reverse1; }
    if (reverse1) { r = numColors - r; }

    if (s == 0) { reverse2 = !reverse2; }
    if (reverse2) { s = numSpreads - s; }

    currentColor = round( r * colorSep );
    spread = s*2 + 1 + 10;
    
    //Serial.println(spread);
    
    uint8_t dot [3] = { 0, 0, currentColor};
    uint8_t bg  [3] = { 128, 0, 128};
    dotsOnColor(dot, bg, spacing, spread, true, 1);

    q++;
  }

  rainbow(20);

}

// Show dot colored dots on bg color background
// dots are spacing pixels apart and move every w milliseconds
void dotsOnColor(uint8_t dot[], uint8_t bg[], uint16_t spacing, uint16_t spread, boolean symmetrical, uint16_t w) {

  // handle each offset s
  for(uint16_t j=1; j<=spacing; j++) {
    // handle each dot at position i+sx
    for(uint16_t i=j-1; i<strip.numPixels(); i=i+spacing) {
      setDot(dot, bg, i, spread, symmetrical);
    }
    strip.show();
    
    delay( floor(w * ( spacing / 2 / j + 0.5 ) ) );
    //delay( pow( abs( j - floor((spacing-0)/2 )), 2 ) );
  }

}

void setDot( uint8_t dot[], uint8_t bg[], uint16_t pixel, uint8_t spread, boolean symmetrical ) {
  if( symmetrical ) {
    spread = ceil(spread / 2);
  }
  // must be signed int or we will roll over and never go negative to stop the loop
  for(int16_t i=spread+1; i>=0; i--) {
    // percent should be 100 when i=0, 0 when i=spread
    uint8_t percent = (uint8_t)( 100 * ( spread  - i ) / spread );
    strip.setPixelColor( getPixel(pixel, -i), getMixedColor( dot, bg, percent ) );
    if( symmetrical ) {
      strip.setPixelColor( getPixel(pixel, i), getMixedColor( dot, bg, percent ) );
    }
  }
}

uint16_t getPixel(uint16_t pos, int16_t offset) {
  int16_t target = pos + offset;
  if( target < 0 ) {
    return strip.numPixels() + target;
  } else if( target < strip.numPixels() ) {
    return target;
  } else {
    return target - strip.numPixels() + 1;
  }
}

uint32_t getMixedColor( uint8_t dot[], uint8_t bg[], uint8_t percent ) {
  if( percent < 0 || percent > 100 ) { percent = 0; }
  uint8_t mixed [3];
  for(uint8_t i=0; i<3; i++) {
    mixed[i] = (uint8_t)( bg[i] + ( dot[i] - bg[i] ) * percent / 100 );
  }
  return strip.Color(mixed[0], mixed[1], mixed[2]);
}










// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

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

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
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

