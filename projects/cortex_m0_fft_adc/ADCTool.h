/* */
#ifndef ADCTool_h
#define ADCTool_h

#include "Arduino.h"

class ADCTool {
  public:
    volatile bool samples_ready;
    ADCTool(word count=128);
    void handler();
    void stopSampling();
    void startSampling();
    word * samples;
  private:
    word num_samples;
    void syncADC();
    void setupReadPin();
    void enable8MHzClock();
    void setupGenericClock();
    void setupADC();
    void setupInterrupt(byte priority);
    void enableADC();
    void disableADC();
};

#endif
