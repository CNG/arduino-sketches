#include "sound_reactive.h"

#define SAMPLES 6  // Length of buffer for dynamic level adjustment

byte
  peak      = 0,      // Used for falling dot
  dotCount  = 0,      // Frame counter for delaying dot-falling speed
  volCount  = 0;      // Frame counter for storing past volume data
int
  vol[SAMPLES],       // Collection of prior volume samples
  lvl       = 10,      // Current "dampened" audio level
  minLvlAvg = 0,      // For dynamic adjustment of graph low & high
  maxLvlAvg = 512;

uint8_t  _num_groups  = 1;  // Number of pixels in strand
uint16_t _led_per_grp = ( numPixels() / _num_groups );  // Number of pixels in strand
uint8_t  _dc_offset   = 0;  // DC offset in mic signal - if unusure, leave 0
uint8_t  _noise       = 20;  // Noise/hum/interference in mic signal
//uint8_t  _noise       = ( analogRead(_pin_mic) % 9 ) * 20;
uint8_t  _top_offset  = (_led_per_grp + 2); // Allow dot to go slightly off scale
uint8_t  _fall_rate   = 6;  // Rate of peak falling dot

void setMeterPixelColor(uint16_t n, uint32_t c){
  for(uint8_t i = 0; i < _num_groups; i++) {
    if( i % 2 == 0 ) {
      setPixelColor( (n + i * _led_per_grp ),c);
    } else {
      setPixelColor( (n + i * _led_per_grp ),c);
    }
  }
}

void pad( int number, byte width = 3 ) {
  if (number >= 0) {
    Serial.print(" ");
  } else {
    Serial.print("-");
    number = 0 - number;
  }
  int currentMax = 10;
  for (byte i=1; i<width; i++){
    if (number < currentMax) {
      Serial.print("0");
    }
    currentMax *= 10;
  }
  Serial.print(number);
}

void volMeter(uint8_t startColor, uint8_t endColor){
  if( checkButton() ){ return; };
  setBrightness();
  startColor = startColor + getOpt(_pin_opt, 0, 255); // wraparound OK
  endColor = endColor + getOpt(_pin_opt, 0, 255); // wraparound OK
  uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      n, n2, height;

  n2 = n = analogRead(_pin_mic);                        // Raw reading from mic 
  // Center on zero:
  // MAX9814: 3.3V/1.25V*1000 = 378.7878
  // MAX4466: (explain) = 512
  n   = abs(n - 512 - _dc_offset);
  n   = (n <= _noise) ? 0 : (n - _noise);             // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)

  // Serial.print(millis());Serial.print(": ");
  // pad(n2);Serial.print(" ");
  // pad(n);Serial.print(" ");
  // pad(lvl);Serial.println();

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = _top_offset * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

  //if(height < 0L)       height = 0;      // Clip output
  if(height < 2)       height = 2;      // Clip output
  else if(height > _top_offset) height = _top_offset;
  if(height > peak)     peak   = height; // Keep 'peak' dot at top

  // Color pixels based on rainbow gradient
  for(i=0; i<_led_per_grp; i++) {
    if(i >= height)               setMeterPixelColor(i,   color(0,   0, 0));
    else setMeterPixelColor(i,Wheel(map(i,0,_led_per_grp-1,startColor,endColor)));
    
  }

  // Draw peak dot  
  if(peak > 0 && peak <= _led_per_grp-1) setMeterPixelColor(peak,Wheel(map(peak,0,_led_per_grp-1,startColor,endColor)));
  
   strip.show(); // Update strip

  // Every few frames, make the peak pixel drop by 1:
    if(++dotCount >= _fall_rate) { //fall rate 
      
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
  if((maxLvl - minLvl) < _top_offset) maxLvl = minLvl + _top_offset;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)
} 

