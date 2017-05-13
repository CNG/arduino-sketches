#define ARM_MATH_CM3
#include <arm_math.h>

#include "fht_setup.h"
#include "led_setup.h"
#include "beat_detection.h"

void pad( int number, byte width = 3 ) {
  if (number >= 0) {
    SerialUSB.print(" ");
  } else {
    SerialUSB.print("-");
    number = 0 - number;
  }
  int currentMax = 10;
  for (byte i=1; i<width; i++){
    if (number < currentMax) {
      SerialUSB.print("0");
    }
    currentMax *= 10;
  }
  SerialUSB.print(number);
}

int log_level = 0;

void setup(){

  if( log_level ){
    while (!Serial);    // wait for serialUSB monitor to be opened
    SerialUSB.begin(1); // rate ignored for native port
  }

  fft_setup();
  led_setup();
  
}

int count = 0;

void loop(){

  if(log_level>0){
    SerialUSB.print(millis());
  }

  fft_loop();

  //fft_print_bins();

  if( count == 0 ){ // too jittery if we do the lights every 1/100 second
    uint16_t * buckets = fft_buckets(6, log_level);
    led_show(buckets, log_level);

    //detect_beat(buckets, log_level);

    // uint16_t * bins = get_bins(1);
    // led_show_32(bins, log_level);
  }



  if(log_level>0){
    SerialUSB.println();
  }

  if(++count >= 3) count = 0;

}
