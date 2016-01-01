#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define N_PIXELS_FULL 134
#define N_PIXELS_HALF 67
#define BRIGHTNESS 160
#define N_STRANDS     4
#define MIC_PIN   A9  // Microphone is attached to this analog pin
#define LED_PIN    6  // NeoPixel LED strand is connected to this pin
#define DC_OFFSET  0  // DC offset in mic signal - if unusure, leave 0
#define SAMPLES   10  // Length of buffer for dynamic level adjustment
#define TOP       69  // Allow dot to go slightly off scale
#define PEAK_FALL 4  // Rate of peak falling dot

byte
  peak      = 0,      // Used for falling dot
  dotCount  = 0,      // Frame counter for delaying dot-falling speed
  volCount  = 0;      // Frame counter for storing past volume data
int
  vol[SAMPLES],       // Collection of prior volume samples
  lvl       = 10,     // Current "dampened" audio level
  minLvlAvg = 0,      // For dynamic adjustment of graph low & high
  maxLvlAvg = 512;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(N_PIXELS_FULL, N_STRANDS, LED_PIN,  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,  NEO_GRB            + NEO_KHZ800);
//Adafruit_NeoPixel matrix = Adafruit_NeoPixel(N_PIXELS_FULL*N_STRANDS, LED_PIN, NEO_GRB + NEO_KHZ800);
int noise = 1;
boolean pushed = false;

int switchPin = 0; // switch is connected to pin 0
int lightMode = 2; // how many times the button has been pressed

void setup() {
  pinMode(switchPin, INPUT_PULLUP); // Set the switch pin as input
  attachInterrupt(2, buttonPushed, FALLING); // listen for high to low

  memset(vol, 0, sizeof(vol));
  Serial.begin(9600);
  noise = ( analogRead(MIC_PIN) % 9 ) * 20;
  matrix.begin();
  matrix.setBrightness(BRIGHTNESS);
}

// Instead of changing brightness directly within interrupt, we'll just toggle a
// variable that we'll check for later. This prevents any weirdness due to 
// variable volatility.
void buttonPushed() {
  pushed = true;
  lightMode++;
  if (lightMode == 5) lightMode = 2;
}

void setMeterPixelColor(uint16_t n, uint32_t c){
  for(uint8_t i = 0; i<N_STRANDS; i++){
    if(n+i < N_PIXELS_FULL){
      matrix.drawPixel(n+i,i,c);
      matrix.drawPixel(N_PIXELS_FULL-n-1+i,i,c);
    }else{
      matrix.drawPixel(n+i,i,c);
      matrix.drawPixel(n+i-N_PIXELS_FULL,i,c);
      matrix.drawPixel(N_PIXELS_FULL-n-1+i,i,c);
    }
    //matrix.setPixelColor(n,c);
    //matrix.setPixelColor(N_PIXELS_FULL-n-1,c);
  }
}
void setMeterPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
  setMeterPixelColor(n, matrix.Color(r,g,b));
}
void setNormalPixelColor(uint16_t n, uint32_t c){
  for(uint8_t i = 0; i<N_STRANDS; i++){
    if(n+i < N_PIXELS_FULL){
      matrix.drawPixel(n+i,i,c);
    }else{
      matrix.drawPixel(n+i-N_PIXELS_FULL,i,c);
    }
    //matrix.setPixelColor(n,c);
  }
}
void setNormalPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
  setNormalPixelColor(n, matrix.Color(r,g,b));
}

void modePulseSlow(){
    if(pushed){ pushed = false; return; }
    colorWipe(matrix.Color(0, 0, 0), 0);

    delay(1000);
    if(pushed){ pushed = false; return; }
    pulse2(matrix.Color(255,0,0));
    delay(5000);
//    pulse2(matrix.Color(128,128,0));
//    delay(5000);
    if(pushed){ pushed = false; return; }
    pulse2(matrix.Color(0,255,0));
    delay(5000);
//    pulse2(matrix.Color(0,128,128));
//    delay(5000);
    if(pushed){ pushed = false; return; }
    pulse2(matrix.Color(0,0,255));
//    delay(5000);
//    pulse2(matrix.Color(128,0,128));
//    delay(5000);
//    pulse2(matrix.Color(125,125,125));

    //delay(20);
}
void modePulseFast(){
    
    if(pushed){ pushed = false; return; }
    pulse(matrix.Color(255,0,0));
    if(pushed){ pushed = false; return; }
    pulse(matrix.Color(128,128,0));
    if(pushed){ pushed = false; return; }
    pulse(matrix.Color(0,255,0));
    if(pushed){ pushed = false; return; }
    pulse(matrix.Color(0,128,128));
    if(pushed){ pushed = false; return; }
    pulse(matrix.Color(0,0,255));
    if(pushed){ pushed = false; return; }
    pulse(matrix.Color(128,0,128));
    if(pushed){ pushed = false; return; }
    pulse(matrix.Color(125,125,125));
    //whiteFlash(60);
}
void modeRainbow(){
    //rainbow(1);
    //rainbowCycle(1);
    for(uint16_t j=0; j<32; j++) {
      if(pushed){ pushed = false; return; }
      for(uint16_t i=0; i<N_PIXELS_HALF; i++) {
        setMeterPixelColor(i, Wheel((i+(j*8-1)) & 255));
      }
      matrix.show();
      delay(50);
    }
}

void modeVolMeter(){
  while(true){
    if(pushed){ pushed = false; return; }
    volMeter();
  }
}

void modeAnts(){
      /*
      int times[] = {1,5,2,10};
      int spaces[] = {0,2,3,3,3,3,4,6,10,15,20};
      for(uint16_t j=0; j<sizeof(times); j++) {
        uint8_t wait = times[random(0,sizeof(times))];
        rainbow( 5, wait, spaces[random(0,sizeof(spaces))], j%2==0, random(0,15) == 0, random(0,30) == 0 );
      }
      */
      uint8_t times[]  = {  0, 1, 1, 2, 2, 3, 4 };
      //int spaces[] = {  0, 2, 2, 4, 6,10,16,26,42 };
      uint8_t spaces[] = { 7, 5, 5, 4, 3, 3, 0 };
      for(uint8_t j=0; j<sizeof(times); j++) {
        if(pushed){ pushed = false; return; }
        rainbow2( 5, times[j], spaces[j], j%2==0, random(0,8) == 0 && spaces[j] > 0, random(0,2000) == 0 );
      }
}

void loop() {

  if (lightMode == 0) {
    modePulseSlow();
  } else if (lightMode == 1) {
    modePulseFast();
  } else if (lightMode == 2) {
    modeRainbow();
  } else if (lightMode == 3) {
    modeVolMeter();
  } else if (lightMode == 4) {
    modeAnts();
  }

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<N_PIXELS_FULL; i++) {
    setNormalPixelColor(i, c);
    if ( wait ) {
      matrix.show();
      delay(wait);
    }
  }
  if ( wait == 0 ) {
    matrix.show();
  }
}

void whiteFlash(uint8_t w) {

  for(uint16_t i=0; i<N_PIXELS_FULL; i++) {
      setNormalPixelColor(i, matrix.Color(255, 255, 255));
  }
  matrix.show();
  delay(w);
  for(uint16_t i=0; i<N_PIXELS_FULL; i++) {
      setNormalPixelColor(i, matrix.Color(0, 0, 0));
  }
  matrix.show();
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

void rainbow2(uint8_t mode, uint8_t wait, uint8_t spaces, boolean dir, boolean solid, boolean storm) {
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

    delay(7);
    for(pixel=0; pixel<N_PIXELS_HALF; pixel++) {
      if(mode==1||mode==2){
        // every pixel different color
        setMeterPixelColor(pixel, Wheel((pixel+color) & 255));
      }else if(mode==3||mode==4){
        // every pixel same color
        setMeterPixelColor(pixel, Wheel(color));
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
                  setMeterPixelColor(pixel, Wheel(approx));
                } else {
                  setMeterPixelColor(pixel, Wheel(color));
                }
              } else {
                setMeterPixelColor(pixel, matrix.Color(0, 0, 0));
              }
            } else {
              if( color % chunkSize == ( spaces - space) ) {
                if( solid ){
                  uint16_t approx, colorDist;
                  colorDist = 255 / 6;
                  approx = color - (color % colorDist);
                  setMeterPixelColor(pixel, Wheel(approx));
                } else {
                  setMeterPixelColor(pixel, Wheel(color));
                }
              } else {
                setMeterPixelColor(pixel, matrix.Color(0, 0, 0));
              }
            }
          }
        }
  
      }
    }
    matrix.show();
    delay(wait);
    if(mode==2||mode==4||mode==6){
      for(uint16_t i=0; i<N_PIXELS_FULL; i++) {
        setNormalPixelColor(i, matrix.Color(0, 0, 0));
      }
      matrix.show();
      delay(wait);
    }  }
}

void pulse(uint32_t c) {
  for(uint16_t j=1; j<42; j++) {
    matrix.setBrightness(map(j*6-1,0,255,0,BRIGHTNESS));
    for(uint16_t i=0; i<N_PIXELS_FULL; i++) {
      setNormalPixelColor(i, c);
    }
    matrix.show();
  }
  for(uint16_t j=42; j>0; j--) {
    matrix.setBrightness(map(j*6-1,0,255,0,BRIGHTNESS));
    for(uint16_t i=0; i<N_PIXELS_FULL; i++) {
      setNormalPixelColor(i, c);
    }
    matrix.show();
  }
  matrix.setBrightness(BRIGHTNESS);
}
void pulse2(uint32_t c) {
  for(uint16_t j=1; j<128; j++) {
    matrix.setBrightness(map(j*2-1,0,255,0,BRIGHTNESS));
    for(uint16_t i=0; i<N_PIXELS_FULL; i++) {
      setNormalPixelColor(i, c);
    }
    matrix.show();
  }
  delay(1000);
  for(uint16_t j=64; j>0; j--) {
    matrix.setBrightness(map(j*4-1,0,255,0,BRIGHTNESS));
    for(uint16_t i=0; i<N_PIXELS_FULL; i++) {
      setNormalPixelColor(i, c);
    }
    matrix.show();
  }
  matrix.setBrightness(BRIGHTNESS);
}

void volMeter(){
  uint16_t  i;
  uint16_t minLvl, maxLvl;
  int      n, height;

  int startColor, endColor;
  startColor = 60;
  endColor   = 250;

  n   = analogRead(MIC_PIN); // Raw reading from mic 
  n   = abs(n - 512 - DC_OFFSET); // Center on zero
  n   = (n <= noise) ? 0 : (n - noise); // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3; // "Dampened" reading (else looks twitchy)

  Serial.print(lvl);
  Serial.print(' ');

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);
  Serial.println(height);

  if(height < 0L)       height = 0;      // Clip output
  else if(height > TOP) height = TOP;
  if(height > peak)     peak   = height; // Keep 'peak' dot at top

  // Color pixels based on rainbow gradient
  //setMeterPixelColor(N_PIXELS_HALF-1,Wheel(map(0,0,N_PIXELS_HALF-1,startColor,endColor)));
  //setMeterPixelColor(N_PIXELS_HALF-2,Wheel(map(1,0,N_PIXELS_HALF-1,startColor,endColor)));
  for(i=0; i<N_PIXELS_HALF-2; i++) {
    if(i >= height){ 
      setMeterPixelColor(N_PIXELS_HALF-i-1,   0,   0, 0);
    } else {
      setMeterPixelColor(N_PIXELS_HALF-i,Wheel(map(i,0,N_PIXELS_HALF-1,startColor,endColor)));
    }
  }

  // Draw peak dot  
  if(peak > 0 && peak <= N_PIXELS_HALF-1){
    setMeterPixelColor(N_PIXELS_HALF-peak,Wheel( map(N_PIXELS_HALF-peak,0,N_PIXELS_HALF-1,startColor,endColor) ));
  }
  
  matrix.show(); // Update matrix

  // Every few frames, make the peak pixel drop by 1:
  if(++dotCount >= PEAK_FALL) { 
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
  if((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)

}

//Rainbow Program
void rainbow(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<255; j++) {
    for(i=0; i<50; i++) { // 50 should be N_PIXELS_FULL i think but not chekcing now
      setNormalPixelColor(i, Wheel((i+j) & 255));
    }
    matrix.show();
    delay(wait);
  }
}

// Rainbow Cycle Program - Equally distributed
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< N_PIXELS_FULL; i++) {
      setNormalPixelColor(i, Wheel(((i * 256 / N_PIXELS_FULL) + j) & 255));
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
