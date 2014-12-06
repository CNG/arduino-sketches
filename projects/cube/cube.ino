/*
LED VU meter for Arduino and Adafruit NeoPixel LEDs.

Hardware requirements:
 - Most Arduino or Arduino-compatible boards (ATmega 328P or better).
 - Adafruit Electret Microphone Amplifier (ID: 1063)
 - Adafruit Flora RGB Smart Pixels (ID: 1260)
   OR
 - Adafruit NeoPixel Digital LED strip (ID: 1138)
 - Optional: battery for portable use (else power through USB or adapter)
Software requirements:
 - Adafruit NeoPixel library

Connections:
 - 3.3V to mic amp +
 - GND to mic amp -
 - Analog pin to microphone output (configurable below)
 - Digital pin to LED data input (configurable below)
 See notes in setup() regarding 5V vs. 3.3V boards - there may be an
 extra connection to make and one line of code to enable or disable.

Written by Adafruit Industries.  Distributed under the BSD license.
This paragraph must be included in any redistribution.
*/

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define N_CHUNKS   1  // Number of pixels in strand
#define N_PX_TOT   8  // Number of pixels in strand
#define N_PX_CHK  ( N_PX_TOT / N_CHUNKS )  // Number of pixels in strand
#define MIC_PIN   A9  // Microphone is attached to this analog pin
#define LED_PIN    6  // NeoPixel LED strand is connected to this pin
#define DC_OFFSET  0  // DC offset in mic signal - if unusure, leave 0
#define NOISE     10  // Noise/hum/interference in mic signal
#define SAMPLES   50  // Length of buffer for dynamic level adjustment
#define TOP       (N_PX_CHK + 2) // Allow dot to go slightly off scale
#define PEAK_FALL 10  // Rate of peak falling dot

byte
  peak      = 0,      // Used for falling dot
  dotCount  = 0,      // Frame counter for delaying dot-falling speed
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

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

void colorWipe(uint8_t wait, uint32_t c1, uint32_t c2 = 999, uint32_t c3 = 999, uint32_t c4 = 999, uint32_t c5 = 999, uint32_t c6 = 999, uint32_t c7 = 999, uint32_t c8 = 999, uint32_t c9 = 999, uint32_t c10 = 999, uint32_t c11 = 999, uint32_t c12 = 999, uint32_t c13 = 999, uint32_t c14 = 999, uint32_t c15 = 999, uint32_t c16 = 999);

void setup() {
  //while (!Serial);
  //Serial.begin(9600);
    randomSeed(analogRead(0));

  pinMode(switchPin, INPUT_PULLUP); // Set the switch pin as input
  if( random(0,2) == 0 ){
    matrix.setBrightness(25);
  } else {
    matrix.setBrightness(230);
  }
  pinMode(LED_PIN, OUTPUT); // not sure why this is here
  buttonState = digitalRead(switchPin); // read the initial state

  // This is only needed on 5V Arduinos (Uno, Leonardo, etc.).
  // Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
  // line.  Audio samples are 'cleaner' at 3.3V.
  // COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  // analogReference(EXTERNAL);

  memset(vol, 0, sizeof(vol));
  matrix.begin();

  matrix.setTextWrap(false);
  matrix.setTextColor(colors[0]);

//  matrix.show();
//  int bright = random(0,8);
//  for(int i=0; i<bright; i++){
//    matrix.drawPixel(0,i,matrix.Color(255,255,255));
//  }
//  matrix.setBrightness(bright * 32);
//  matrix.show();
//  delay(1000);

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
      if (lightMode == 6) lightMode = 0;
    }
  }
  buttonState = pinVal; // save the new state in our variable
  int w = 120;

  int w2 = 30 * random(0,3);
  
//lightMode = 3;
  if (lightMode == 0) {
  colorWipe(
    w2,
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 255, 255), // White
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 255, 255)  // White
  );
  delay(w);
  colorWipe(
    w2,
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 255, 255), // White
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 255, 255), // White
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 255, 255), // White
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 255, 255)  // White
  );
  delay(w);
  colorWipe(
    w2,
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 255,   0), // Yellow
    matrix.Color(  0, 255,   0)  // Green
  );
  delay(w);
  colorWipe(
    w2,
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 255,   0), // Yellow
    matrix.Color(  0, 255,   0), // Green
    matrix.Color(  0,   0, 255)  // Blue
  );
  delay(w);
  colorWipe(
    w2,
    matrix.Color(255, 255,   0), // Yellow
    matrix.Color(  0, 255,   0), // Green
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(255,   0,   0)  // Red
  );
  delay(w);
  colorWipe(
    w2,
    matrix.Color(  0, 255,   0), // Green
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 255,   0)  // Yellow
  );
  delay(w);
  colorWipe(
    w2,
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(192,   0, 255), // Indigo
    matrix.Color(255,   0, 255), // Violet
    matrix.Color(255, 255, 255), // White
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 128,   0), // Orange
    matrix.Color(255, 255,   0), // Yellow
    matrix.Color(  0, 255,   0)  // Green
  );
  delay(w);
  colorWipe(
    w2,
    matrix.Color(255,   0, 255), // Violet
    matrix.Color(255, 255, 255), // White
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 128,   0), // Orange
    matrix.Color(255, 255,   0), // Yellow
    matrix.Color(  0, 255,   0), // Green
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(192,   0, 255)  // Indigo
  );
  delay(w);
  colorWipe(
    w2,
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 128,   0), // Orange
    matrix.Color(255, 255,   0), // Yellow
    matrix.Color(  0, 255,   0), // Green
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(192,   0, 255), // Indigo
    matrix.Color(255,   0, 255), // Violet
    matrix.Color(255, 255, 255)  // White
  );
  delay(w);
  colorWipe(
    w2,
    matrix.Color(255, 255,   0), // Yellow
    matrix.Color(  0, 255,   0), // Green
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(192,   0, 255), // Indigo
    matrix.Color(255,   0, 255), // Violet
    matrix.Color(255, 255, 255), // White
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 128,   0)  // Orange
  );
  delay(w);
  colorWipe(
    w2,
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 128,   0), // Orange
    matrix.Color(255, 255,   0), // Yellow
    matrix.Color(  0, 255,   0), // Green
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(192,   0, 255), // Indigo
    matrix.Color(255,   0, 255), // Violet
    matrix.Color(255, 255, 255), // White
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 128,   0), // Orange
    matrix.Color(255, 255,   0), // Yellow
    matrix.Color(  0, 255,   0), // Green
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(192,   0, 255), // Indigo
    matrix.Color(255,   0, 255), // Violet
    matrix.Color(255, 255, 255)  // White
  );
  delay(w);
  colorWipe(
    w2,
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(192,   0, 255), // Indigo
    matrix.Color(255,   0, 255), // Violet
    matrix.Color(255, 255, 255), // White
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 128,   0), // Orange
    matrix.Color(255, 255,   0), // Yellow
    matrix.Color(  0, 255,   0), // Green
    matrix.Color(  0,   0, 255), // Blue
    matrix.Color(192,   0, 255), // Indigo
    matrix.Color(255,   0, 255), // Violet
    matrix.Color(255, 255, 255), // White
    matrix.Color(255,   0,   0), // Red
    matrix.Color(255, 128,   0), // Orange
    matrix.Color(255, 255,   0), // Yellow
    matrix.Color(  0, 255,   0)  // Green
  );
  delay(w);

  //colorWipe(30, 0);
//  matrix.fillScreen(0);
//  matrix.show();

//    colorWipe(0, matrix.Color(0, 0, 0));
//    colorWipe(30, matrix.Color(255, 0, 0)); // Red
//    colorWipe(30, matrix.Color(255, 255, 0)); // Yellow
//    colorWipe(30, matrix.Color(0, 255, 0)); // Green
//    //colorWipe(30, matrix.Color(0, 255, 255)); // LightBlue
//    colorWipe(30, matrix.Color(0, 0, 255)); // Blue
//    colorWipe(30, matrix.Color(255, 0, 255)); // Purple
//    colorWipe(1, matrix.Color(255, 255, 255)); // White
  } else if (lightMode == 1) {
    rainbow(1);
    //rainbowCycle(1);
  } else {
    int counter = 0;
    while (counter < 100) {
      volMeter(lightMode);
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

void volMeter(int lightMode){
  uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      n, height;
  int startColor, endColor;
  if (lightMode == 2) {
    startColor = 120;
    endColor   = 220;
  }
  if (lightMode == 3) {
    startColor = 220;
    endColor   = 250;
  }
  if (lightMode == 4) {
    startColor = 20;
    endColor   = 60;
  }
  if (lightMode == 5) {
    startColor = 1;
    endColor   = 255;
  }

  n   = analogRead(MIC_PIN);                        // Raw reading from mic 
  n   = abs(n - 512 - DC_OFFSET); // Center on zero
  n   = (n <= NOISE) ? 0 : (n - NOISE);             // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

  if(height < 0L)       height = 0;      // Clip output
  else if(height > TOP) height = TOP;
  if(height > peak)     peak   = height; // Keep 'peak' dot at top

  // Color pixels based on rainbow gradient
//  for(i=0; i<N_PX_CHK; i++) {
//    if(i >= height)               setPixelColor(i,   0,   0, 0);
//    else setPixelColor(i,Wheel(map(i,0,N_PX_CHK-1,startColor,endColor)));
//    
//  }

  // Draw peak dot  
//  if(peak > 0 && peak <= N_PX_CHK-1) setPixelColor(peak,Wheel(map(peak,0,N_PX_CHK-1,startColor,endColor)));

//matrix.fillScreen(Wheel(map(height,0,N_PX_CHK-1,120,220)));
matrix.fillScreen( matrix.Color(min(255,map(lvl,0,150,0,8)*32),0,min(255,map(lvl,0,150,8,0)*32)) );
  //Serial.println(lvl);

   matrix.show(); // Update strip
   delay(7);

  // Every few frames, make the peak pixel drop by 1:
    if(++dotCount >= PEAK_FALL) { //fall rate 
      
      if(peak > 0) peak--;
      dotCount = 0;
    }

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
    matrix.drawPixel(x, (n),c);
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
