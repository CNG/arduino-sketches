#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  //strip.setBrightness(128);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  /*
  for(uint16_t i=0; i<3; i++) {
    rainbow(5,1,0,true);
    for(uint16_t j=0; j<i; j++) { whiteFlash(60); }
  }
  for(uint16_t i=0; i<4; i++) {
    rainbow(1,1,0,true);
    for(uint16_t j=0; j<i; j++) { whiteFlash(60); }
  }
  for(uint16_t i=0; i<3; i++) {
    rainbow(5,1,0,true);
  }
  for(uint16_t i=0; i<5; i++) {
    rainbow(1,1,0,true);
  }
  for(uint16_t i=0; i<15; i++) {
    whiteFlash(60);
  }
  */
  for(uint16_t i=0; i<10; i++) {
    /*
    int times[] = {1,5,2,10};
    int spaces[] = {0,2,3,3,3,3,4,6,10,15,20};
    for(uint16_t j=0; j<sizeof(times); j++) {
      uint8_t wait = times[random(0,sizeof(times))];
      rainbow( 5, wait, spaces[random(0,sizeof(spaces))], j%2==0, random(0,15) == 0, random(0,30) == 0 );
    }
    */
    uint8_t times[]  = {  0, 1, 1, 2, 2, 3, 4, 6, 8,13 };
    //int spaces[] = {  0, 2, 2, 4, 6,10,16,26,42 };
    uint8_t spaces[] = { 15,10, 8, 7, 5, 5, 4, 3, 3, 0 };
    for(uint16_t j=0; j<sizeof(times); j++) {
      uint8_t wait   = times[j];
      uint8_t space  = spaces[j];
      rainbow( 5, wait, space, j%2==0, random(0,4) == 0 && space > 0, false );
    }
  }
  
  //  rainbowBlink(50);
}

void rainbow(uint8_t mode, uint8_t wait, uint8_t spaces, boolean dir, boolean solid, boolean storm) {
  uint16_t color, pixel;
  for(color=0; color<256; color++) {

    if( storm ) {
      if( color % 50 == 0 ) {
        uint8_t flashes = random(1,9);
        for( uint8_t p = 0; p<=flashes; p++ ){
          whiteFlash(60);
        }
        color = color + flashes;
        continue;
      }
    }

    for(pixel=0; pixel<strip.numPixels(); pixel++) {
      if(mode==1||mode==2){
        // every pixel different color
        strip.setPixelColor(pixel, Wheel((pixel+color) & 255));
      }else if(mode==3||mode==4){
        // every pixel same color
        strip.setPixelColor(pixel, Wheel(color));
      }else if(mode==5||mode==6){
  
        uint8_t chunkSize = spaces + 1;
        // need to loop through num spaces but only trigger on one pixel
        for( uint8_t space = 0; space<=spaces; space++ ){
          if( pixel % chunkSize == space ){
            if( dir ){
              if( color % chunkSize == space ) {
                if( solid ){
                  uint16_t approx, colorDist;
                  colorDist = 255 / 6;
                  approx = color - (color % colorDist);
                  strip.setPixelColor(pixel, Wheel(approx));
                } else {
                  strip.setPixelColor(pixel, Wheel(color));
                }
              } else {
                strip.setPixelColor(pixel, strip.Color(0, 0, 0));
              }
            } else {
              if( color % chunkSize == ( spaces - space) ) {
                if( solid ){
                  uint16_t approx, colorDist;
                  colorDist = 255 / 6;
                  approx = color - (color % colorDist);
                  strip.setPixelColor(pixel, Wheel(approx));
                } else {
                  strip.setPixelColor(pixel, Wheel(color));
                }
              } else {
                strip.setPixelColor(pixel, strip.Color(0, 0, 0));
              }
            }
          }
        }
  
      }
    }
    strip.show();
    delay(wait);
    if(mode==2||mode==4||mode==6){
      for(uint8_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
      strip.show();
      delay(wait);
    }  }
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

void whiteFlash(uint8_t w) {

  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 255, 255));
  }
  strip.show();
  delay(w);
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  delay(w);


}
void niceColors(uint8_t w) {
  colorWipe(strip.Color(255, 0, 0), w); // Red
  colorWipe(strip.Color(0, 0, 0), w);
  colorWipe(strip.Color(255, 255, 0), w);
  colorWipe(strip.Color(0, 0, 0), w);
  colorWipe(strip.Color(0, 255, 0), w); // Green
  colorWipe(strip.Color(0, 0, 0), w);
  colorWipe(strip.Color(0, 255, 255), w);
  colorWipe(strip.Color(0, 0, 0), w);
  colorWipe(strip.Color(0, 0, 255), w); // Blue
  colorWipe(strip.Color(0, 0, 0), w);
  colorWipe(strip.Color(255, 0, 255), w);
  colorWipe(strip.Color(0, 0, 0), w);  
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbowBlink(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256/16; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j*16) & 255));
    }
    strip.show();
    delay(wait);
    strip.setPixelColor(i, 0);
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

