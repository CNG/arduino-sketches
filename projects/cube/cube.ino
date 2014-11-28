#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 4, 1, PIN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_PROGRESSIVE +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_TILE_PROGRESSIVE,
  NEO_GRB + NEO_KHZ800);

void setup() {
  matrix.begin();
  matrix.setBrightness(64);
  matrix.show(); // Initialize all pixels to 'off'
}

void loop() {
  //rainbow(1);
  colorWipe(matrix.Color(255, 0, 0), 30); // Red
  colorWipe(matrix.Color(255, 255, 0), 30); // Yellow
  colorWipe(matrix.Color(0, 255, 0), 30); // Green
  //colorWipe(matrix.Color(0, 255, 255), 30); // LightBlue
  colorWipe(matrix.Color(0, 0, 255), 30); // Blue
  colorWipe(matrix.Color(255, 0, 255), 30); // Purple
  colorWipe(matrix.Color(255, 255, 255), 30); // White

}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  int16_t x, y;
  for(y=matrix.height()-1; y>=0; y--){
    for(x=0; x<matrix.width(); x++){
      matrix.drawPixel(x, y, c);
    }
    matrix.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t c, x;
  int16_t y;
  for(c=0; c<256; c++) {
    for(y=matrix.height()-1; y>=0; y--){
      for(x=0; x<matrix.width(); x++){
        matrix.drawPixel(x, y, Wheel((y+c) & 255));
      }
    }
    matrix.show();
    delay(wait);
  }
}

/*

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
*/
//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    //for (int q=0; q > 3; q++) {
    for (int q=3; q <= 0; q--) {
        for (int i=0; i < matrix.height() ; i=i+3) {
          for (int k=0; k < matrix.width(); k++) {
            matrix.drawPixel( k, i+q, Wheel( (i+j) % 255));    //turn every third pixel on
          }
        }
        matrix.show();
       
        delay(wait);
       
        for (int i=0; i < matrix.height() ; i=i+3) {
          for (int k=0; k < matrix.width(); k++) {
            matrix.drawPixel( k, i+q, 0);        //turn every third pixel off
          }
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

