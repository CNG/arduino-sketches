/**
 * VolumeMeter.cpp - Library for volume meters
 * Created by Charlie Gorichanaz.
 * Adapted from code by Adafruit Industries:
 *   LED VU meter for Arduino and Adafruit NeoPixel LEDs.
 * Distributed under the BSD license.
 * This paragraph must be included in any redistribution.
 */
#include "VolumeMeter.h"


VolumeMeter::VolumeMeter(byte pin, word height, word rate, word count){
  // Parameters
  sensor = pin;
  max_height = height + 2;  // Allow peak off edge
  peak_fall_rate  = rate;
  num_samples = count;

  // Hardcoded settings (candidates for parametrization)
  dc_offset = 0;  // Leave 0 if unusure
  noise = ( analogRead(sensor) % 9 ) * 20;

  // Other starting values
  peak_fall_counter = 0;
  sample_count = 0;
  ave_min_level = 0;
  ave_max_level = 512;
  level = max_height / 3;  // arbitrary
  // Now array length is known; allocate full samples[num_samples]
  samples = (word *) malloc(num_samples * sizeof(word));
}


Levels VolumeMeter::sample(){
  static word
    min_level, max_level,
    reading,
    height,  // Meter height
    peak,  // Peak dot height
    i;

  reading = analogRead(sensor);  // Raw reading from sensor
  reading = abs(reading - 512 - dc_offset);  // Center on zero
  reading = (reading <= noise) ? 0 : (reading - noise);  // Remove noise/hum
  level = ((level * 7) + reading) >> 3;  // "Dampened" reading (else looks twitchy)

  // Calculate bar height based on dynamic min/max levels (fixed point)
  height = max_height * (level - ave_min_level) / (long)(ave_max_level - ave_min_level);
  if(height < 0L) height = 0;  // Clip output
  else if(height > max_height) height = max_height;
  if(height > peak) peak = height;  // Keep 'peak' dot at top

  // Every few frames, make the peak pixel drop by 1
  if(++peak_fall_counter >= peak_fall_rate) { 
    if(peak > 0) peak--;
    peak_fall_counter = 0;
  }

  samples[sample_count] = reading;  // Save sample for dynamic leveling
  if(++sample_count >= num_samples) sample_count = 0;  // Rollover sample counter

  // Get volume range of prior frames
  min_level = max_level = samples[0];
  for(i=1; i<num_samples; i++) {
    if(samples[i] < min_level)      min_level = samples[i];
    else if(samples[i] > max_level) max_level = samples[i];
  }
  // min_level and max_level indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if((max_level - min_level) < max_height) max_level = min_level + max_height;
  ave_min_level = (ave_min_level * 63 + min_level) >> 6; // Dampen min/max levels
  ave_max_level = (ave_max_level * 63 + max_level) >> 6; // (fake rolling average)

  return (Levels){height, peak};
}
