
#include <Adafruit_NeoPixel.h>

#define N_PIXELS  45  // Number of pixels in strand
#define MIC_PIN   A7  // Microphone is attached to this analog pin
#define LED_PIN    1  // NeoPixel LED strand is connected to this pin
#define DC_OFFSET  0  // DC offset in mic signal - if unusure, leave 0
#define SAMPLES   50  // Length of buffer for dynamic level adjustment
#define TOP       28  // Allow dot to go slightly off scale
#define PEAK_FALL 20  // Rate of peak falling dot

byte
  peak      = 0,      // Used for falling dot
  dotCount  = 0,      // Frame counter for delaying dot-falling speed
  volCount  = 0;      // Frame counter for storing past volume data
int
  vol[SAMPLES],       // Collection of prior volume samples
  lvl       = 10,     // Current "dampened" audio level
  minLvlAvg = 0,      // For dynamic adjustment of graph low & high
  maxLvlAvg = 512;
Adafruit_NeoPixel
  strip = Adafruit_NeoPixel(N_PIXELS*4, LED_PIN, NEO_GRB + NEO_KHZ800);
int noise = 1;

int switchPin = 10; // switch is connected to pin 10
int pinVal; // variable for reading the pin status
int pinVal2;
int buttonState; // variable to hold the button state
int lightMode = 0; // how many times the button has been pressed

void setup() {
  pinMode(switchPin, INPUT_PULLUP); // Set the switch pin as input
  pinMode(LED_PIN, OUTPUT); // not sure why this is here
  buttonState = digitalRead(switchPin); // read the initial state

  memset(vol, 0, sizeof(vol));
  //Serial.begin(9600);
  noise = ( analogRead(MIC_PIN) % 9 ) * 20;
  strip.begin();
}

void setMeterPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
  strip.setPixelColor(n,r,g,b);
  strip.setPixelColor(89-n,r,g,b);
  strip.setPixelColor(90+n,r,g,b);
  strip.setPixelColor(179-n,r,g,b);
}
void setMeterPixelColor(uint16_t n, uint32_t c){
  strip.setPixelColor(n,c);
  strip.setPixelColor(89-n,c);
  strip.setPixelColor(90+n,c);
  strip.setPixelColor(179-n,c);
}
void setNormalPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
  strip.setPixelColor(n,r,g,b);
  strip.setPixelColor(179-n,r,g,b);
}
void setNormalPixelColor(uint16_t n, uint32_t c){
  strip.setPixelColor(n,c);
  strip.setPixelColor(179-n,c);
}

void loop() {
    int counter = 0;
    while (counter < 1000) {
      volMeter(lightMode);
      counter++;
    }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    if ( wait ) {
      strip.show();
      delay(wait);
    }
  }
  if ( wait == 0 ) {
    strip.show();
  }
}

void pulse(uint32_t c) {
  for(uint16_t j=1; j<64; j++) {
    strip.setBrightness(j*4-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
  }
  for(uint16_t j=64; j>0; j--) {
    strip.setBrightness(j*4-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
  }
  strip.setBrightness(255);
}

void volMeter(int lightMode){
  lightMode = lightMode - 1;
  uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      n, height;

  n   = analogRead(MIC_PIN); // Raw reading from mic 
  n   = abs(n - 512 - DC_OFFSET); // Center on zero
  n   = (n <= noise) ? 0 : (n - noise); // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3; // "Dampened" reading (else looks twitchy)

  //Serial.println(noise);

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

  if(height < 0L)       height = 0;      // Clip output
  else if(height > TOP) height = TOP;
  if(height > peak)     peak   = height; // Keep 'peak' dot at top

  // Color pixels based on rainbow gradient
  for(i=0; i<N_PIXELS; i++) {
    if(i >= height){ 
      setMeterPixelColor(N_PIXELS-i,   0,   0, 0);
    } else {
      setMeterPixelColor(N_PIXELS-i,Wheel(i));
    }
  }

  // Draw peak dot  
  if(peak > 0 && peak <= N_PIXELS-1){
    setMeterPixelColor(N_PIXELS-peak,Wheel(N_PIXELS-peak));
  }
  
  strip.show(); // Update strip

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
  for(j=0; j<25; j++) {
    for(i=0; i<50; i++) {
      setNormalPixelColor(i, Wheel((i+j) & 25));
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
  if(WheelPos<8) {
    return strip.Color(0,0,255);
  } else if(WheelPos<15){
    return strip.Color(255,0,0);
  } else {
    return strip.Color(85,85,85);
  }
}
