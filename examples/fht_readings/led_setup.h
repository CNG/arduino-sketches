extern void pad( int number, byte width );

#define ROWS       8  // NeoPixel LED strand is connected to this pin
#define COLS      32  // NeoPixel LED strand is connected to this pin
#define LED_PIN   22  // NeoPixel LED strand is connected to this pin
#define SAMPLES   128  // Length of buffer for dynamic level adjustment
#define BRIGHTNESS 10

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

void led_show(void* param){
  uint16_t *bucket;
  bucket=(uint16_t*) param;

  char s[100];
  sprintf(s,"%05d %05d %05d %05d %05d %05d", bucket[0], bucket[1], bucket[2], bucket[3], bucket[4], bucket[5]);
  SerialUSB.println(s);

  matrix.fillScreen(matrix.Color(0, 0, 0));
  for (int i = 0; i < 6; i++) {  // For each of the 6 useful octave bins

    // Calculate bar height based on dynamic min/max levels (fixed point):
    height[i] = ROWS * ( bucket[i] - minLvlAvg[i] ) / (long)( maxLvlAvg[i] - minLvlAvg[i] );
    if(height[i] < 0L) height[i] = 0; // Clip output
    else if(height[i] > ROWS) height[i] = ROWS;

    vol[i][volCount] = bucket[i]; // Save sample for dynamic leveling
 
    // Get volume range of prior frames
    minLvl[i] = maxLvl[i] = vol[i][0];
    for(uint8_t j=1; j<SAMPLES; j++) {
      if(vol[i][j] < minLvl[i])      minLvl[i] = vol[i][j];
      else if(vol[i][j] > maxLvl[i]) maxLvl[i] = vol[i][j];
    }
    if((maxLvl[i] - minLvl[i]) < ROWS) maxLvl[i] = minLvl[i] + ROWS;
    minLvlAvg[i] = (minLvlAvg[i] * 63 + minLvl[i]) >> 6; // Dampen min/max levels
    maxLvlAvg[i] = (maxLvlAvg[i] * 63 + maxLvl[i]) >> 6; // (fake rolling average)

    for(int y = 0; y < ROWS ; y++){
      if( y < height[i] ){
        matrix.drawPixel(5*i+1,y,matrix.Color(200, 0, 255));
        matrix.drawPixel(5*i+2,y,matrix.Color(200, 0, 255));
        matrix.drawPixel(5*i+3,y,matrix.Color(200, 0, 255));
        matrix.drawPixel(5*i+4,y,matrix.Color(200, 0, 255));
        matrix.drawPixel(5*i+5,y,matrix.Color(200, 0, 255));
      }
    }

  }
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter

  matrix.show();
}

void led_show_32(void* param){
  uint16_t *bucket;
  bucket=(uint16_t*) param;

  matrix.fillScreen(matrix.Color(0, 0, 0));
  for (int i = 1; i <= 32; i++) {

    // Calculate bar height based on dynamic min/max levels (fixed point):
    height[i] = ROWS * ( bucket[i*2] - minLvlAvg[i] ) / (long)( maxLvlAvg[i] - minLvlAvg[i] );
    if(height[i] < 0L) height[i] = 0; // Clip output
    else if(height[i] > ROWS) height[i] = ROWS;

    vol[i][volCount] = bucket[i*2]; // Save sample for dynamic leveling
 
    // Get volume range of prior frames
    minLvl[i] = maxLvl[i] = vol[i][0];
    for(uint8_t j=1; j<SAMPLES; j++) {
      if(vol[i][j] < minLvl[i])      minLvl[i] = vol[i][j];
      else if(vol[i][j] > maxLvl[i]) maxLvl[i] = vol[i][j];
    }
    if((maxLvl[i] - minLvl[i]) < ROWS) maxLvl[i] = minLvl[i] + ROWS;
    minLvlAvg[i] = (minLvlAvg[i] * 63 + minLvl[i]) >> 6; // Dampen min/max levels
    maxLvlAvg[i] = (maxLvlAvg[i] * 63 + maxLvl[i]) >> 6; // (fake rolling average)

    for(int y = 0; y < ROWS ; y++){
      if( y < height[i] ){
        matrix.drawPixel(i,y,matrix.Color(200, 0, 255));
      }
    }

  }
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter

  matrix.show();
}