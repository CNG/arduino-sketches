
#include <Adafruit_NeoPixel.h>

#define PIXELS_PER_LINE 59
#define PIXELS_PER_SIDE ceil(PIXELS_PER_LINE/2)

Adafruit_NeoPixel
  strip = Adafruit_NeoPixel(PIXELS_PER_LINE*2, 6, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(64);
}

void setMeterPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
  setNormalPixelColor(PIXELS_PER_SIDE-n,r,g,b);
  setNormalPixelColor(PIXELS_PER_SIDE+n,r,g,b);
}
void setMeterPixelColor(uint16_t n, uint32_t c){
  setNormalPixelColor(PIXELS_PER_SIDE-n,c);
  setNormalPixelColor(PIXELS_PER_SIDE+n,c);
}
void setNormalPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
  strip.setPixelColor(n,r,g,b);
  strip.setPixelColor(PIXELS_PER_LINE*2-n,r,g,b);
}
void setNormalPixelColor(uint16_t n, uint32_t c){
  strip.setPixelColor(n,c);
  strip.setPixelColor(PIXELS_PER_LINE*2-n,c);
}

void mode0(){
    colorWipe(strip.Color(0, 0, 0), 0);

    delay(1000);
    pulse2(strip.Color(255,0,0));
    delay(5000);
//    pulse2(strip.Color(128,128,0));
//    delay(5000);
    pulse2(strip.Color(0,255,0));
    delay(5000);
//    pulse2(strip.Color(0,128,128));
//    delay(5000);
    pulse2(strip.Color(0,0,255));
//    delay(5000);
//    pulse2(strip.Color(128,0,128));
//    delay(5000);
//    pulse2(strip.Color(125,125,125));

    //delay(20);
}
void mode1(){
    
    pulse(strip.Color(255,0,0));
    pulse(strip.Color(128,128,0));
    pulse(strip.Color(0,255,0));
    pulse(strip.Color(0,128,128));
    pulse(strip.Color(0,0,255));
    pulse(strip.Color(128,0,128));
    pulse(strip.Color(125,125,125));
}
void mode2(){
    //rainbow(1);
    //rainbowCycle(1);
    for(uint16_t j=0; j<32; j++) {
      for(uint16_t i=0; i<PIXELS_PER_LINE; i++) {
        setMeterPixelColor(i, Wheel((i+(j*8-1)) & 255));
      }
      strip.show();
      delay(50);
    }
}
void mode3(){
  }
void mode4(){
}
void mode5(){
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
        rainbow2( 5, wait, space, j%2==0, random(0,8) == 0 && space > 0, false, random(0,1) );
      }
    }
}

void loop() {

  rainbow(0);

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<PIXELS_PER_LINE; i++) {
    setMeterPixelColor(i, c);
    if ( wait ) {
      strip.show();
      delay(wait);
    }
  }
  if ( wait == 0 ) {
    strip.show();
  }
}

void whiteFlash(uint8_t w) {

  for(uint16_t i=0; i<PIXELS_PER_LINE*2; i++) {
      setNormalPixelColor(i, strip.Color(255, 255, 255));
  }
  strip.show();
  delay(w);
  for(uint16_t i=0; i<PIXELS_PER_LINE*2; i++) {
      setNormalPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  delay(w);


}

void pad( int number, byte width = 3 ) {
 int currentMax = 10;
 for (byte i=1; i<width; i++){
   if (number < currentMax) {
     Serial.print("0");
   }
   currentMax *= 10;
 }
 Serial.print(number);
}

void rainbow2(uint8_t mode, uint8_t wait, uint8_t spaces, boolean dir, boolean solid, boolean storm, uint8_t mapping) {
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

    for(pixel=0; pixel<PIXELS_PER_LINE; pixel++) {
      if(mode==1||mode==2){
        // every pixel different color
        setNormalPixelColor(pixel, Wheel((pixel+color) & 255));
      }else if(mode==3||mode==4){
        // every pixel same color
        setNormalPixelColor(pixel, Wheel(color));
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
                  setNormalPixelColor(pixel, Wheel(approx));
                } else {
                  setNormalPixelColor(pixel, Wheel(color));
                }
              } else {
                setNormalPixelColor(pixel, strip.Color(0, 0, 0));
              }
            } else {
              if( color % chunkSize == ( spaces - space) ) {
                if( solid ){
                  uint16_t approx, colorDist;
                  colorDist = 255 / 6;
                  approx = color - (color % colorDist);
                  setNormalPixelColor(pixel, Wheel(approx));
                } else {
                  setNormalPixelColor(pixel, Wheel(color));
                }
              } else {
                setNormalPixelColor(pixel, strip.Color(0, 0, 0));
              }
            }
          }
        }
  
      }
    }
    strip.show();
    delay(wait);
    if(mode==2||mode==4||mode==6){
      for(uint8_t i=0; i<PIXELS_PER_LINE; i++) {
        setNormalPixelColor(i, strip.Color(0, 0, 0));
      }
      strip.show();
      delay(wait);
    }  }
}

void pulse(uint32_t c) {
  for(uint16_t j=1; j<42; j++) {
    strip.setBrightness(j*6-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
  }
  for(uint16_t j=42; j>0; j--) {
    strip.setBrightness(j*6-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
  }
  strip.setBrightness(255);
}
void pulse2(uint32_t c) {
  for(uint16_t j=1; j<128; j++) {
    strip.setBrightness(j*2-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
  }
  delay(1000);
  for(uint16_t j=64; j>0; j--) {
    strip.setBrightness(j*4-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
  }
  strip.setBrightness(255);
}


//Rainbow Program
void rainbow(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<255; j++) {
    for(i=0; i<PIXELS_PER_LINE; i++) {
      setNormalPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Rainbow Cycle Program - Equally distributed
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
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
