#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

uint8_t _pin_led = 6; // NeoPixel LED strand is connected to this pin
int _num_led_cols = 2; // Number of columns of LEDs
int _num_led_rows = 60; // Number of pixels in each column

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
  _num_led_rows, _num_led_cols, _pin_led,
  NEO_MATRIX_BOTTOM  + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix strip = matrix;
void setVerticalPixelColor(uint16_t n, uint32_t c){
  for(uint8_t i = 0; i < _num_led_cols; i++) {
    matrix.drawPixel(n, i, c);
  }
}
void setHorizontalPixelColor(uint16_t n, uint32_t c){
  for(uint8_t i = 0; i < _num_led_rows; i++) {
    matrix.drawPixel(n, i, c);
  }
}
void setPixelColor(uint16_t n, uint32_t c){
  setVerticalPixelColor(n, c);
}
int numPixels(){
  return _num_led_rows;
}
void show(){
  matrix.show();
}
void setBrightness(uint8_t n){
  matrix.setBrightness(n);
}
uint16_t color(uint8_t r, uint8_t g, uint8_t b){
  matrix.Color(r, g, b);
}


void setup() {
  matrix.begin();
  //strip.setBrightness(30);
  matrix.show(); // Initialize all pixels to 'off'
}

void pulse(uint32_t c) {
  for(uint16_t j=0; j<255; j=j+2) {
    strip.setBrightness(j);
    for(uint16_t i=0; i<numPixels(); i++) {
      setPixelColor(i, c);
    }
    strip.show();
  }
  for(uint16_t j=255; j>0; j=j-2) {
    strip.setBrightness(j);
    for(uint16_t i=0; i<numPixels(); i++) {
      setPixelColor(i, c);
    }
    strip.show();
  }
  for(uint16_t i=0; i<numPixels(); i++) {
    setPixelColor(i, strip.Color(0,0,0));
  }
  strip.show();
  //delay(1000);
  strip.setBrightness(255);
}

void loop() {
//while(1){
//    pulse(Wheel(random(0,255)));
//}
//
//    pulse(strip.Color(255,0,0));
//    pulse(strip.Color(128,128,0));
//    pulse(strip.Color(0,255,0));
//    pulse(strip.Color(0,128,128));
//    pulse(strip.Color(0,0,255));
//    pulse(strip.Color(128,0,128));
//    pulse(strip.Color(125,125,125));

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
      rainbow( 5, wait*10, space, j%2==0, random(0,8) == 0 && space > 0, random(0,10000) == 0 );
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

    for(pixel=0; pixel<numPixels(); pixel++) {
      if(mode==1||mode==2){
        // every pixel different color
        setPixelColor(pixel, Wheel((pixel+color) & 255));
      }else if(mode==3||mode==4){
        // every pixel same color
        setPixelColor(pixel, Wheel(color));
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
                  setPixelColor(pixel, Wheel(approx));
                } else {
                  setPixelColor(pixel, Wheel(color));
                }
              } else {
                setPixelColor(pixel, strip.Color(0, 0, 0));
              }
            } else {
              if( color % chunkSize == ( spaces - space) ) {
                if( solid ){
                  uint16_t approx, colorDist;
                  colorDist = 255 / 6;
                  approx = color - (color % colorDist);
                  setPixelColor(pixel, Wheel(approx));
                } else {
                  setPixelColor(pixel, Wheel(color));
                }
              } else {
                setPixelColor(pixel, strip.Color(0, 0, 0));
              }
            }
          }
        }
  
      }
    }
    strip.show();
    delay(wait);
    if(mode==2||mode==4||mode==6){
      for(uint8_t i=0; i<numPixels(); i++) {
        setPixelColor(i, strip.Color(0, 0, 0));
      }
      strip.show();
      delay(wait);
    }  }
}



//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < numPixels(); i=i+3) {
        setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < numPixels(); i=i+3) {
        setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void whiteFlash(uint8_t w) {

  for(uint16_t i=0; i<numPixels(); i++) {
      setPixelColor(i, strip.Color(255, 255, 255));
  }
  strip.show();
  delay(w);
  for(uint16_t i=0; i<numPixels(); i++) {
      setPixelColor(i, strip.Color(0, 0, 0));
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
  for(uint16_t i=0; i<numPixels(); i++) {
      setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbowBlink(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256/16; j++) {
    for(i=0; i<numPixels(); i++) {
      setPixelColor(i, Wheel((i+j*16) & 255));
    }
    strip.show();
    delay(wait);
    setPixelColor(i, 0);
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< numPixels(); i++) {
      setPixelColor(i, Wheel(((i * 256 / numPixels()) + j) & 255));
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

