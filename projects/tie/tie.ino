#include <Adafruit_NeoPixel.h>

#define N_PIXELS  185  // Number of pixels in strand
#define MIC_PIN   A5  // Microphone is attached to this analog pin
#define LED_PIN    5  // NeoPixel LED strand is connected to this pin
#define DC_OFFSET  400  // DC offset in mic signal - if unusure, leave 0
#define NOISE     100  // Noise/hum/interference in mic signal
#define SAMPLES   20  // Length of buffer for dynamic level adjustment
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
  strip = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
int noise = 1;

void setup() {
  memset(vol, 0, sizeof(vol));
  //Serial.begin(9600);
  noise = ( analogRead(MIC_PIN) % 9 ) * 20;
  strip.begin();
}

void loop() {
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
    if(i >= height)               strip.setPixelColor(N_PIXELS-i,   0,   0, 0);
    // else strip.setPixelColor(N_PIXELS-i,Wheel(map(i,0,strip.numPixels()-1,30,150)));
    else {
      strip.setPixelColor(N_PIXELS-i,Wheel(i));
    }
  }

  // Draw peak dot  
  if(peak > 0 && peak <= N_PIXELS-1) strip.setPixelColor(N_PIXELS-peak,Wheel(N_PIXELS-peak));
  //if(peak > 0 && peak <= N_PIXELS-1) strip.setPixelColor(N_PIXELS-peak,Wheel(peak));
  
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
