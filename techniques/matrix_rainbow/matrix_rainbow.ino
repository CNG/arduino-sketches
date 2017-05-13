#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 6,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);
void setup() {
  matrix.begin();
  //matrix.setTextWrap(false);
  matrix.setBrightness(80);
  //matrix.setTextColor(colors[0]);
}

void loop() {
  rainbow(10);
  //matrix.fillScreen(matrix.Color(0, 255, 0));
  //matrix.drawPixel(0,0, matrix.Color(32, 32, 32));
}

void drawRow(uint8_t row, uint16_t color) {
  uint16_t i;
  for(i=0; i<8; i++){
    matrix.drawPixel(i,row,color);
  }
}

void drawDiag(uint8_t row, uint16_t color) {
  uint16_t i, j;
  for(i=0; i<8; i++){
    for(j=0; j<8; j++){
      matrix.drawPixel(i,j,color);
    }
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<8; i++) {
      drawRow(i, Wheel((i+j) & 255));
    }
    matrix.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

