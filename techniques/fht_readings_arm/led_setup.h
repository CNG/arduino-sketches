#define ROWS       8  // NeoPixel LED strand is connected to this pin
#define COLS      32  // NeoPixel LED strand is connected to this pin
#define LED_PIN   22  // NeoPixel LED strand is connected to this pin
#define SAMPLES   128  // Length of buffer for dynamic level adjustment
#define BRIGHTNESS 32

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(COLS, ROWS, LED_PIN,
  NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

byte volCount = 0;

int
  vol[6][SAMPLES],
  minLvlAvg[6],
  maxLvlAvg[6],
  minLvl[6],
  maxLvl[6],
  height[6];

/*
  Input a value 0 to 255 to get a color value.
  The colors are a transition r - g - b - back to r.
 */
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

// uint32_t colors[6] = {
//   Wheel(180),
//   Wheel(150),
//   Wheel(210),
//   Wheel(230),
//   Wheel(120),
//   Wheel(80)
// };

void led_setup() {
  for( uint8_t i = 0; i<6; i++ ){
    minLvlAvg[i] = 0;      // For dynamic adjustment of graph low & high
    maxLvlAvg[i] = 16;
  }
  matrix.begin();
  matrix.setBrightness(BRIGHTNESS);
  matrix.fillScreen(matrix.Color(0, 0, 0));
  matrix.show();


}

void led_show(uint16_t * buckets, uint8_t log_level){

  if(log_level==1){
    char s[100];
    sprintf(s," %05d %05d %05d %05d %05d %05d", buckets[0], buckets[1], buckets[2], buckets[3], buckets[4], buckets[5]);
    SerialUSB.print(s);
  }

  matrix.fillScreen(matrix.Color(0, 0, 0));
  for (uint8_t i = 0; i < 6; i++) {  // For each of the 6 useful octave bins

    // Calculate bar height based on dynamic min/max levels (fixed point):
    height[i] = ROWS * ( buckets[i] - minLvlAvg[i] ) / (long)( maxLvlAvg[i] - minLvlAvg[i] );
    if(height[i] < 0L) height[i] = 0; // Clip output
    else if(height[i] > ROWS) height[i] = ROWS;

    vol[i][volCount] = buckets[i]; // Save sample for dynamic leveling
 
    // Get volume range of prior frames
    minLvl[i] = maxLvl[i] = vol[i][0];
    for(uint8_t j=1; j<SAMPLES; j++) {
      if(vol[i][j] < minLvl[i])      minLvl[i] = vol[i][j];
      else if(vol[i][j] > maxLvl[i]) maxLvl[i] = vol[i][j];
    }
    if((maxLvl[i] - minLvl[i]) < ROWS) maxLvl[i] = minLvl[i] + ROWS;
    minLvlAvg[i] = (minLvlAvg[i] * 63 + minLvl[i]) >> 6; // Dampen min/max levels
    maxLvlAvg[i] = (maxLvlAvg[i] * 63 + maxLvl[i]) >> 6; // (fake rolling average)

    for(uint8_t y = 0; y < ROWS ; y++){
      if( y < height[i] ){
        // matrix.drawPixel(5*i+1,y,colors[i]);
        // matrix.drawPixel(5*i+2,y,colors[i]);
        // matrix.drawPixel(5*i+3,y,colors[i]);
        // matrix.drawPixel(5*i+4,y,colors[i]);
        // matrix.drawPixel(5*i+5,y,colors[i]);
        matrix.drawPixel(5*i+1,y,Wheel(map(5*i,0,32,210,100)));
        matrix.drawPixel(5*i+2,y,Wheel(map(5*i,0,32,210,100)));
        matrix.drawPixel(5*i+3,y,Wheel(map(5*i,0,32,210,100)));
        matrix.drawPixel(5*i+4,y,Wheel(map(5*i,0,32,210,100)));
        matrix.drawPixel(5*i+5,y,Wheel(map(5*i,0,32,210,100)));
      }
    }

  }
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter

  matrix.show();
}

void led_show_32(uint16_t * buckets, uint8_t log_level){

  matrix.fillScreen(matrix.Color(0, 0, 0));
  for (uint8_t i = 0; i < 32; i++) {

    // Calculate bar height based on dynamic min/max levels (fixed point):
    height[i] = ROWS * ( buckets[i] - minLvlAvg[i] ) / (long)( maxLvlAvg[i] - minLvlAvg[i] );
    if(height[i] < 0L) height[i] = 0; // Clip output
    else if(height[i] > ROWS) height[i] = ROWS;

    vol[i][volCount] = buckets[i]; // Save sample for dynamic leveling
 
    // Get volume range of prior frames
    minLvl[i] = maxLvl[i] = vol[i][0];
    for(uint8_t j=1; j<SAMPLES; j++) {
      if(vol[i][j] < minLvl[i])      minLvl[i] = vol[i][j];
      else if(vol[i][j] > maxLvl[i]) maxLvl[i] = vol[i][j];
    }
    if((maxLvl[i] - minLvl[i]) < ROWS) maxLvl[i] = minLvl[i] + ROWS;
    minLvlAvg[i] = (minLvlAvg[i] * 63 + minLvl[i]) >> 6; // Dampen min/max levels
    maxLvlAvg[i] = (maxLvlAvg[i] * 63 + maxLvl[i]) >> 6; // (fake rolling average)

    for(uint8_t y = 0; y < ROWS ; y++){
      if( y < height[i] ){
        //matrix.drawPixel(i,y,matrix.Color(200, 0, 255));
        matrix.drawPixel(i,y,Wheel(map(i,0,32,210,100)));
      }
    }

  }
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter

  matrix.show();
}

