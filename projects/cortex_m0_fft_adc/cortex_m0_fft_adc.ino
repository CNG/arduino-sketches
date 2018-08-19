/*
Some code originally from https://github.com/stimmer/DueVGA/blob/master/extras/Waterfall/waterfall/waterfall.ino
*/

// #define DEBUG
#ifdef DEBUG
#define PRINT(x) Serial.print (x)
#define PRINTBIN(x) Serial.print (x, BIN)
#define PRINTLN(x) Serial.println (x)
#else
#define PRINT(x)
#define PRINTLN(x)
#endif

byte log_level = 1;

#include <FFTTool.h>

const word FFT_exponent = 2;  // for FFT_size 512
const word FFT_size = 128 * pow(2, FFT_exponent);
const byte num_buckets = 6;
FFTTool fft (FFT_size);


void ADC_Handler() {
  HANDLER_HELPER fft.handler();
}


void setup() {
  #ifdef DEBUG
  Serial.begin(115200);
  while(!Serial);
  #endif
  fft.setup();
  PRINTLN("Starting");
}


void loop() {
  // static unsigned long start_time = millis();
  // static unsigned long loops = 0;
  // static bool warming = true;
  // if (warming && loops == 200) {
  //   start_time = millis();
  //   loops = 0;
  //   warming = false;
  // }

  fft.loop();
  //fft.print_bins(24);
  word * buckets = fft.buckets(num_buckets, log_level);

  // if (++loops % 50 == 0 && !warming) {
  //   unsigned long
  //     samples = loops * FFT_size,
  //     total_millis = millis() - start_time,
  //     samples_per_second = samples * 1000 / total_millis,
  //     max_detected_frequency = samples_per_second / 2,
  //     bandwidth_per_bin = max_detected_frequency / FFT_size * 2,
  //     first_bucket_bandwidth = bandwidth_per_bin * (FFT_size / 2 / (1 << (num_buckets - 1)));
  //   PRINT(samples_per_second); PRINT(" samples_per_second; ");
  //   PRINT(max_detected_frequency); PRINT(" max_detected_frequency; ");
  //   PRINT(bandwidth_per_bin); PRINT(" bandwidth_per_bin; ");
  //   PRINT(first_bucket_bandwidth); PRINT(" first_bucket_bandwidth; ");
  //   PRINTLN();
  // }
}



