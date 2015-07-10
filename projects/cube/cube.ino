#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#define MIC_PIN   A9  // Microphone is attached to this analog pin
#define LED_PIN    6  // NeoPixel LED strand is connected to this pin
#define NOISE      5  // Noise/hum/interference in mic signal
#define SAMPLES   60  // Length of buffer for dynamic level adjustment
#define TOP       32
#define MAXBRIGHT 200
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

int switchPin = 10; // switch is connected to pin 10
int pinVal; // variable for reading the pin status
int pinVal2;
int buttonState; // variable to hold the button state
int lightMode = 0; // how many times the button has been pressed

const uint16_t colors[] = { matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };
const uint32_t clrbla =  matrix.Color(  0,   0,   0); // Black
const uint32_t clryel =  matrix.Color(255, 255,   0); // Yellow
const uint32_t clrgre =  matrix.Color(  0, 255,   0); // Green
const uint32_t clrblu =  matrix.Color(  0,   0, 255); // Blue
const uint32_t clrind =  matrix.Color(192,   0, 255); // Indigo
const uint32_t clrvio =  matrix.Color(255,   0, 255); // Violet
const uint32_t clrwhi =  matrix.Color(255, 255, 255); // White
const uint32_t clrred =  matrix.Color(255,   0,   0); // Red
const uint32_t clrora =  matrix.Color(255, 128,   0); // Orange

void colorWipe(uint8_t wait, uint32_t c1, uint32_t c2 = 999, uint32_t c3 = 999, uint32_t c4 = 999, uint32_t c5 = 999, uint32_t c6 = 999, uint32_t c7 = 999, uint32_t c8 = 999, uint32_t c9 = 999, uint32_t c10 = 999, uint32_t c11 = 999, uint32_t c12 = 999, uint32_t c13 = 999, uint32_t c14 = 999, uint32_t c15 = 999, uint32_t c16 = 999);

void setup() {
  matrix.begin();
  matrix.fillScreen(matrix.Color(0, 0, 0));
  matrix.show();

  pinMode(switchPin, INPUT_PULLUP); // Set the switch pin as input
  pinMode(LED_PIN, OUTPUT); // not sure why this is here
  buttonState = digitalRead(switchPin); // read the initial state
  memset(vol, 0, sizeof(vol));

  //while (!Serial);
  //Serial.begin(9600);

  // SINGLE BRIGHTNESS
  matrix.setBrightness(40);

  // RANDOM LOW/HIGH BRIGHTNESS
  //  randomSeed(analogRead(0));
  //  if( random(0,2) == 0 ){
  //    matrix.setBrightness(25);
  //  } else {
  //    matrix.setBrightness(100);
  //  }

  matrix.setTextWrap(false);
  matrix.setTextColor(colors[0]);
  int x    = matrix.width();
  int pass = 0;
  while(pass < 1){
    matrix.fillScreen(0);
    matrix.setCursor(x, 0);
    //int numb = ;
    //matrix.print(numb);
    matrix.print(F("HAPPY CUBE"));
    if(--x < -36) {
      x = matrix.width();
      if(++pass >= 3) pass = pass;
      matrix.setTextColor(colors[pass]);
    }
    matrix.show();
    delay(50);
  }

}

void loop() {
  pinVal = digitalRead(switchPin); // read input value and store it in pinVal
  delay (20);
  pinVal2 = digitalRead(switchPin);
  if (pinVal == pinVal2) {
    if (pinVal != buttonState && pinVal==LOW) { // the button state has changed!
      lightMode++;
      if (lightMode == 3) lightMode = 0;
    }
  }
  buttonState = pinVal; // save the new state in our variable

  int w = 120;

  int w2 = 30 * random(0,3);
  
  //lightMode = 2;

  if (lightMode == 0) {
  colorWipe(w2, clrblu, clrblu, clrblu, clrblu, clrred, clrwhi, clrred, clrwhi); delay(w);
  colorWipe(w2, clrblu, clrblu, clrblu, clrblu, clrred, clrwhi, clrred, clrwhi, clrblu, clrblu, clrblu, clrblu, clrred, clrwhi, clrred, clrwhi); delay(w);
  colorWipe(w2, clrblu, clrred, clryel, clrgre); delay(w);
  colorWipe(w2, clrred, clryel, clrgre, clrblu); delay(w);
  colorWipe(w2, clryel, clrgre, clrblu, clrred); delay(w);
  colorWipe(w2, clrgre, clrblu, clrred, clryel); delay(w);
  colorWipe(w2, clrblu, clrind, clrvio, clrwhi, clrred, clrora, clryel, clrgre); delay(w);
  colorWipe(w2, clrvio, clrwhi, clrred, clrora, clryel, clrgre, clrblu, clrind); delay(w);
  colorWipe(w2, clrred, clrora, clryel, clrgre, clrblu, clrind, clrvio, clrwhi); delay(w);
  colorWipe(w2, clryel, clrgre, clrblu, clrind, clrvio, clrwhi, clrred, clrora); delay(w);
  colorWipe(w2, clrred, clrora, clryel, clrgre, clrblu, clrind, clrvio, clrwhi, clrred, clrora, clryel, clrgre, clrblu, clrind, clrvio, clrwhi); delay(w);
  colorWipe(w2, clrblu, clrind, clrvio, clrwhi, clrred, clrora, clryel, clrgre, clrblu, clrind, clrvio, clrwhi, clrred, clrora, clryel, clrgre); delay(w);

  // colorWipe(30, 0);
  // matrix.fillScreen(0);
  // matrix.show();

   colorWipe( 0, clrbla);
   colorWipe(30, clrred);
   colorWipe(30, clryel);
   colorWipe(30, clrgre);
   colorWipe(30, clrblu);
   colorWipe( 1, clrwhi);


  } else if (lightMode == 1) {
    rainbow(1);
    //rainbowCycle(1);
  } else if (lightMode == 2) {
    matrix.setBrightness(75);
    int counter = 0;
    while (counter < 100) {
      volMeter(false);
      counter++;
    }
  } else if (lightMode == 3) {
    matrix.setBrightness(75);
    int counter = 0;
    while (counter < 100) {
      volMeter(true);
      counter++;
    }
  }

}

// Fill the dots one after the other with a color
void colorWipe(uint8_t wait, uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4, uint32_t c5, uint32_t c6, uint32_t c7, uint32_t c8, uint32_t c9, uint32_t c10, uint32_t c11, uint32_t c12, uint32_t c13, uint32_t c14, uint32_t c15, uint32_t c16) {
  int16_t x, y;
  uint8_t sections = 1;
       if( c16 != 999 ) { sections = 16; }
  else if( c15 != 999 ) { sections = 15; }
  else if( c14 != 999 ) { sections = 14; }
  else if( c13 != 999 ) { sections = 13; }
  else if( c12 != 999 ) { sections = 12; }
  else if( c11 != 999 ) { sections = 11; }
  else if( c10 != 999 ) { sections = 10; }
  else if(  c9 != 999 ) { sections = 9;  }
  else if(  c8 != 999 ) { sections = 8;  }
  else if(  c7 != 999 ) { sections = 7;  }
  else if(  c6 != 999 ) { sections = 6;  }
  else if(  c5 != 999 ) { sections = 5;  }
  else if(  c4 != 999 ) { sections = 4;  }
  else if(  c3 != 999 ) { sections = 3;  }
  else if(  c2 != 999 ) { sections = 2;  }
  float sectionSize = matrix.width() / sections;
  
  for(y=matrix.height()-1; y>=0; y--){
    for(x=0; x<matrix.width(); x++){
           if( x < round(sectionSize*1)  ) { matrix.drawPixel(x, y, c1);  }
      else if( x < round(sectionSize*2)  ) { matrix.drawPixel(x, y, c2);  }
      else if( x < round(sectionSize*3)  ) { matrix.drawPixel(x, y, c3);  }
      else if( x < round(sectionSize*4)  ) { matrix.drawPixel(x, y, c4);  }
      else if( x < round(sectionSize*5)  ) { matrix.drawPixel(x, y, c5);  }
      else if( x < round(sectionSize*6)  ) { matrix.drawPixel(x, y, c6);  }
      else if( x < round(sectionSize*7)  ) { matrix.drawPixel(x, y, c7);  }
      else if( x < round(sectionSize*8)  ) { matrix.drawPixel(x, y, c8);  }
      else if( x < round(sectionSize*9)  ) { matrix.drawPixel(x, y, c9);  }
      else if( x < round(sectionSize*10) ) { matrix.drawPixel(x, y, c10); }
      else if( x < round(sectionSize*11) ) { matrix.drawPixel(x, y, c11); }
      else if( x < round(sectionSize*12) ) { matrix.drawPixel(x, y, c12); }
      else if( x < round(sectionSize*13) ) { matrix.drawPixel(x, y, c13); }
      else if( x < round(sectionSize*14) ) { matrix.drawPixel(x, y, c14); }
      else if( x < round(sectionSize*15) ) { matrix.drawPixel(x, y, c15); }
      else if( x < round(sectionSize*16) ) { matrix.drawPixel(x, y, c16); }
    }
    if( wait != 0 ){
      matrix.show();
      delay(wait);
    }
  }
  if( wait == 0 ){
    matrix.show();
  }
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

}
void setLevelSpectrum(uint8_t level, int endColor, int startColor){

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

  uint8_t templevel = 0;
  // = matrix.Color(255, 0, 255);
  matrix.fillScreen(matrix.Color(0, 0, 0));
  for(uint8_t x=0; x < cols; x++){
    for(uint8_t y=0; y < rows; y++){
      if(bitmap[x][y] == 1){
        templevel++;
        uint8_t mx = cols - x - 1;
        uint8_t my = rows - y - 1;
        uint32_t color = Wheel(map(templevel,0,TOP,endColor,startColor));
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

}

const uint8_t rows2 = 4;
const uint8_t cols2 = 4;
int bitmap2[cols2][rows2]; // we'll say 0:0 is the lower left for now

void setLevel2(uint8_t level, uint32_t color){

  level = ceil(level / 2);

  // set all array spots to 0
  for(uint8_t x = 0; x < cols2; x++){
    for(uint8_t y = 0; y < rows2; y++){
      bitmap2[x][y] = 0;
    }
  }

  for(uint8_t i = 0; i < level; i++){
    for(int8_t y = 0; y < rows2; y++){
      for(int8_t x = 0; x < cols2; x++){
        if(bitmap2[x][y] == 0 && ( x-2 < 0 || y+1 >= rows2 || bitmap2[x-2][y+1] == 1) ){
          bitmap2[x][y] = 1;
          goto final;
        }
      }
    }
    final: ;
  }

  // = matrix.Color(255, 0, 255);
  matrix.fillScreen(matrix.Color(0, 0, 0));
  for(uint8_t x=0; x < cols2; x++){
    for(uint8_t y=0; y < rows2; y++){
      if(bitmap2[x][y] == 1){
        uint8_t mx = cols2 - x - 1;
        uint8_t my = rows2 - y - 1;
        matrix.drawPixel(mx                  , my, color);
        matrix.drawPixel(mx + 7 - 2 * mx     , my, color);
        matrix.drawPixel(mx                  , my + 7 - 2 * my, color);
        matrix.drawPixel(mx + 7 - 2 * mx     , my + 7 - 2 * my, color);

        matrix.drawPixel(mx              +  8, my, color);
        matrix.drawPixel(mx + 7 - 2 * mx +  8, my, color);
        matrix.drawPixel(mx              +  8, my + 7 - 2 * my, color);
        matrix.drawPixel(mx + 7 - 2 * mx +  8, my + 7 - 2 * my, color);

        matrix.drawPixel(mx              + 16, my, color);
        matrix.drawPixel(mx + 7 - 2 * mx + 16, my, color);
        matrix.drawPixel(mx              + 16, my + 7 - 2 * my, color);
        matrix.drawPixel(mx + 7 - 2 * mx + 16, my + 7 - 2 * my, color);

        matrix.drawPixel(mx              + 24, my, color);
        matrix.drawPixel(mx + 7 - 2 * mx + 24, my, color);
        matrix.drawPixel(mx              + 24, my + 7 - 2 * my, color);
        matrix.drawPixel(mx + 7 - 2 * mx + 24, my + 7 - 2 * my, color);
      }
    }
  }
  matrix.show();

}

void volMeter(bool choice){
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
  if(choice){
    setLevel(height, Wheel(map(height,0,TOP,endColor,startColor)));
  }else{
    setLevelSpectrum(height,endColor,startColor);
    //setLevel2(height, Wheel(map(height,0,TOP,endColor,startColor)));
  }
  
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

void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
  n = matrix.height() - n - 1;
  uint16_t x;
    for(x=0; x<matrix.width(); x++){
      matrix.drawPixel(x, (n),matrix.Color(r,g,b));
    }
}

void setPixelColor(uint16_t n, uint32_t c){
  n = matrix.height() - n - 1;
  uint16_t x;
  for(x=0; x<matrix.width(); x++){
    matrix.drawPixel( x, n, c );
  }
}

//Rainbow Program
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

// Rainbow Cycle Program - Equally distributed
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< matrix.numPixels(); i++) {
      matrix.setPixelColor(i, Wheel(((i * 256 / matrix.numPixels()) + j) & 255));
    }
    matrix.show();
    delay(wait);
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
