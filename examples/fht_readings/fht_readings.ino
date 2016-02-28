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

int log_level = 2;

void setup(){

  while (!Serial);    // wait for serialUSB monitor to be opened
  SerialUSB.begin(1); // rate ignored for native port

  fft_setup();
  led_setup();
  
}


void loop(){

  if(log_level>0){
    SerialUSB.print(millis());
  }

  fft_loop();

  //fft_print_bins();

  void* bucket = fft_buckets(6, 1);
  //led_show(bucket);

  detect_beat(bucket, 2);

  // void* bins = get_bins();
  // led_show_32(bins);


  if(log_level>0){
    SerialUSB.println();
  }

}
