/* */
#ifndef FFTTool_h
#define FFTTool_h

#include "Arduino.h"
#include "ADCTool.h"
#define ARM_MATH_CM0PLUS
#include <arm_math.h>

// Without following two statements, arm_rfft_q15() segfaults! ?!?!
#define HANDLER_HELPER static volatile bool silly; silly = true;

void pad( int number, byte width = 3 );

class FFTTool {
  private:
    ADCTool adc;
    word num_samples;
    void init(word count);
  public:
    arm_rfft_instance_q15 S;
    q15_t * out;
    q15_t * outsq;
    FFTTool(
      word count=128
      )
      :
      adc(count)
      {
        init(count);
      }
    void setup();
    void loop();
    void handler();
    void print_bins(word max=NULL);
    uint16_t * buckets(uint8_t buckets_n, uint8_t log_level);
};

#endif
