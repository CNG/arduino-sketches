/**
 * VolumeMeter.h - Library for volume meters
 * Created by Charlie Gorichanaz.
 * Adapted from code by Adafruit Industries:
 *   LED VU meter for Arduino and Adafruit NeoPixel LEDs.
 * Distributed under the BSD license.
 * This paragraph must be included in any redistribution.
 */
#ifndef VolumeMeter_h
#define VolumeMeter_h

#include "Arduino.h"

struct Levels {
  int height, peak;
};

class VolumeMeter {
  public:
    VolumeMeter(byte pin, word height, word rate, word samples);
    Levels sample();
    word max_height;  // Total meter height
  private:
    byte sensor;
    word
      dc_offset,  // DC offset in mic signal
      noise,  // Line noise level
      peak_fall_rate,  // Peak falls slower than main height.
      num_samples,  // Dynamic level adjustment buffer length
      peak_fall_counter,  // Frame counter for delaying dot-falling speed
      sample_count,  // Frame counter for storing past volume data
      ave_min_level,  // For dynamic adjustment of graph low & high
      ave_max_level,
      level;  // Current "dampened" audio level
    // Collection of prior volume samples
    word * samples;  // samples is pointer to "word array" (word *)
};

#endif
