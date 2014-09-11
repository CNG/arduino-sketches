#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(480, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

//  Serial.begin(9600); while (! Serial);
//  unsigned long lasttime = millis();
//  uint8_t dot [3] = { 255,   0,   0};
//  uint8_t bg  [3] = {   0,   0,  63};

//  for(int i=0; i<1000; i++) { setDot(dot, bg, 400, 10, false); }
//  Serial.print("Finished setDot  loop: "); Serial.println(millis() - lasttime); lasttime = millis();
//  for(int i=0; i<1000; i++) { setDot2(dot, bg, 400, 10, false); }
//  Serial.print("Finished setDot2 loop: "); Serial.println(millis() - lasttime); lasttime = millis();

//  for(int i=0; i<1000; i++) { getPixel(400, -5); }
//  Serial.print("Finished getPixel loop: "); Serial.println(millis() - lasttime); lasttime = millis();

//  uint8_t percent = round( 100 * ( 4  - 3 ) / 4 );
//  for(int i=0; i<10000; i++) { getMixedColor( dot, bg, percent ); }
//  Serial.print("Finished getMixedColor loop: "); Serial.println(millis() - lasttime); lasttime = millis();

}

void loop() {

/*
  uint16_t index = 0;

  while(true) {
    uint8_t stretch = random(1,120);
    uint32_t color = Wheel(random(0,255));
    for(uint8_t i=0; i<stretch; i++) {
      strip.setPixelColor(index, color);
      strip.show();
      index++;
      if(index==strip.numPixels()) {
        index = 0;
      }
    }
  }
*/



  uint8_t dot [3] = { 255,   0,     0};
  uint8_t bg  [3] = {   0,   0,   255};

  dotsOnColor(dot, bg, 60, 5);

  //colorWipe(strip.Color(255,0,0), 1);
  //rainbow(20);
  //delay(1000);
}

// Show dot colored dots on bg color background
// dots are spacing pixels apart and move every w milliseconds
void dotsOnColor(uint8_t dot[], uint8_t bg[], uint16_t spacing, uint16_t w) {

/*
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(bg[0], bg[1], bg[2]));
  }
  strip.show();
*/

  uint8_t spread = 8;
  // handle each offset s
  for(uint16_t j=1; j<=spacing; j++) {
    // handle each dot at position i+sx
    for(uint16_t i=j-1; i<strip.numPixels(); i=i+spacing) {
      setDot(dot, bg, i, spread, false);
    }
    strip.show();
    
    delay( floor(w * ( spacing / 2 / j + 0.5 ) ) );
    //delay( pow( abs( j - floor((spacing-0)/2 )), 2 ) );
  }

}

void setDot( uint8_t dot[], uint8_t bg[], uint16_t pixel, uint8_t spread, boolean symmetrical ) {
  // must be signed int or we will roll over and never go negative to stop the loop
  for(int16_t i=spread; i>=0; i--) {
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

