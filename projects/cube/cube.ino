#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#define MIC_PIN   A9  // Microphone is attached to this analog pin
#define LED_PIN    6  // NeoPixel LED strand is connected to this pin
#define NOISE      5  // Noise/hum/interference in mic signal
#define SAMPLES   60  // Length of buffer for dynamic level adjustment
#define TOP       32
#define MAXBRIGHT 100
byte
  volCount  = 0;      // Frame counter for storing past volume data
int
  vol[SAMPLES],       // Collection of prior volume samples
  lvl       = 10,      // Current "dampened" audio level
  minLvlAvg = 0,      // For dynamic adjustment of graph low & high
  maxLvlAvg = 512;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 4, 1, LED_PIN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_PROGRESSIVE +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_TILE_PROGRESSIVE,
  NEO_GRB + NEO_KHZ800);

void setup() {
  matrix.begin();
  matrix.fillScreen(matrix.Color(0, 0, 0));
  matrix.show();
  //while (!Serial);
  //Serial.begin(9600);
  //randomSeed(analogRead(0));
  matrix.setBrightness(MAXBRIGHT);
  memset(vol, 0, sizeof(vol));
}

void loop() {
  volMeter();
}


const uint8_t rows = 8;
const uint8_t cols = 4;
int bitmap[cols][rows]; // we'll say 0:0 is the lower left for now

void setLevel(uint8_t level, uint32_t color){

  // set all array spots to 0
  for(uint8_t x = 0; x < cols; x++){
    for(uint8_t y = 0; y < rows; y++){
      bitmap[x][y] = 0;
    }
  }

  for(uint8_t i = 0; i < level; i++){
    for(int8_t y = 0; y < rows; y++){
      for(int8_t x = 0; x < cols; x++){
        if(bitmap[x][y] == 0 && ( x-2 < 0 || y+1 >= rows || bitmap[x-2][y+1] == 1) ){
          bitmap[x][y] = 1;
          goto final;
        }
      }
    }
    final: ;
  }

  // = matrix.Color(255, 0, 255);
  matrix.fillScreen(matrix.Color(0, 0, 0));
  for(uint8_t x=0; x < cols; x++){
    for(uint8_t y=0; y < rows; y++){
      if(bitmap[x][y] == 1){
        uint8_t mx = cols - x - 1;
        uint8_t my = rows - y - 1;
        matrix.drawPixel(mx                  , my, color);
        matrix.drawPixel(mx + 7 - 2 * mx     , my, color);
        matrix.drawPixel(mx              +  8, my, color);
        matrix.drawPixel(mx + 7 - 2 * mx +  8, my, color);
        matrix.drawPixel(mx              + 16, my, color);
        matrix.drawPixel(mx + 7 - 2 * mx + 16, my, color);
        matrix.drawPixel(mx              + 24, my, color);
        matrix.drawPixel(mx + 7 - 2 * mx + 24, my, color);
      }
    }
  }
  matrix.show();

  // Serial.println("");
  // Serial.print(bitmap[0][7]); Serial.print(bitmap[1][7]); Serial.print(bitmap[2][7]); Serial.println(bitmap[3][7]);
  // Serial.print(bitmap[0][6]); Serial.print(bitmap[1][6]); Serial.print(bitmap[2][6]); Serial.println(bitmap[3][6]);
  // Serial.print(bitmap[0][5]); Serial.print(bitmap[1][5]); Serial.print(bitmap[2][5]); Serial.println(bitmap[3][5]);
  // Serial.print(bitmap[0][4]); Serial.print(bitmap[1][4]); Serial.print(bitmap[2][4]); Serial.println(bitmap[3][4]);
  // Serial.print(bitmap[0][3]); Serial.print(bitmap[1][3]); Serial.print(bitmap[2][3]); Serial.println(bitmap[3][3]);
  // Serial.print(bitmap[0][2]); Serial.print(bitmap[1][2]); Serial.print(bitmap[2][2]); Serial.println(bitmap[3][2]);
  // Serial.print(bitmap[0][1]); Serial.print(bitmap[1][1]); Serial.print(bitmap[2][1]); Serial.println(bitmap[3][1]);
  // Serial.print(bitmap[0][0]); Serial.print(bitmap[1][0]); Serial.print(bitmap[2][0]); Serial.println(bitmap[3][0]);

}

void volMeter(){
  uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      n, height;
  int startColor, endColor;

  startColor = 120;  endColor   = 220;
  startColor = 85;  endColor   = 255;

  n   = analogRead(MIC_PIN);
  n   = abs(n - 512); // Center on zero
  n   = (n <= NOISE) ? 0 : (n - NOISE); // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3; // "Dampened" reading (else looks twitchy)
  //lvl = n;

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);
  if(height < 0L)       height = 0;      // Clip output
  else if(height > TOP) height = TOP;


  matrix.setBrightness( map(height,0,TOP,MAXBRIGHT,MAXBRIGHT/4) );
  setLevel(height, Wheel(map(height,0,TOP,endColor,startColor)));
  
  //Serial.print( lvl ); Serial.print( ' ' ); Serial.println( height ); 

//  Color pixels based on rainbow gradient
//  for(i=0; i<N_PX_CHK; i++) {
//    if(i >= height)               setPixelColor(i,   0,   0, 0);
//    else setPixelColor(i,Wheel(map(i,0,N_PX_CHK-1,startColor,endColor)));
//    
//  }


//matrix.fillScreen(Wheel(map(height,0,N_PX_CHK-1,120,220)));
//matrix.fillScreen( matrix.Color(min(255,map(lvl,0,150,0,8)*32),0,min(255,map(lvl,0,150,8,0)*32)) );
  //Serial.println(lvl);

   //matrix.show();

   delay(7);


  vol[volCount] = n;                      // Save sample for dynamic leveling
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter

  // Get volume range of prior frames
  minLvl = maxLvl = vol[0];
  for(i=1; i<SAMPLES; i++) {
    if(vol[i] < minLvl)      minLvl = vol[i];
    else if(vol[i] > maxLvl) maxLvl = vol[i];
  }
  // minLvl and maxLvl indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)
}

void setPixelColor(uint16_t n, uint32_t c){
  n = matrix.height() - n - 1;
  uint16_t x;
  for(x=0; x<matrix.width(); x++){
    matrix.drawPixel( x, n, c );
  }
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
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
