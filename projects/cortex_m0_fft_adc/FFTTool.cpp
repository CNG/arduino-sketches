#include "FFTTool.h"

void FFTTool::init(word count){
  num_samples = count;
  // Now array length is known; allocate full out[num_samples]
  out = (q15_t *) malloc(num_samples * sizeof(q15_t));
  // Now array length is known; allocate full outsq[num_samples * 2]
  outsq = (q15_t *) malloc(num_samples * sizeof(q15_t) * 2);
}


void FFTTool::loop(){
  static word sample;

  while (!adc.samples_ready);
  adc.samples_ready = false;

  for(sample = 0; sample < num_samples; sample++){
    out[sample] = (adc.samples[sample] - 2048) << 4;
  }
  arm_rfft_q15(&S, out, outsq);
  arm_cmplx_mag_squared_q15(outsq, out, num_samples/2);
}


void FFTTool::handler(){
  adc.handler();
}


void FFTTool::setup(){
  adc.startSampling();
  arm_rfft_init_q15(&S, num_samples, false, true);
}


void FFTTool::print_bins(word max){
  if (!max) { max = num_samples / 2; }
  Serial.print(millis());
  for( int i=1;i<max; i++){
    pad(out[i], 4);
    if( i == max - 1 ){
      Serial.println();
    } else {
      Serial.print(" ");
    }
  }
}


void pad( int number, byte width ) {
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


// convert num_samples/2 bins to $buckets_n summed buckets as in [Scheirer 1998]
// buckets will be like 200Hz, 400, 800, 1600, 3200, 6400
uint16_t * FFTTool::buckets(uint8_t buckets_n, uint8_t log_level){
  // # bins in first bucket, 2^n == 1 << n
  uint16_t group_n = num_samples/2 / ( 1 << ( buckets_n - 1 ) );
  // static so we can return address of local variable to outside function
  static uint16_t* buckets = (uint16_t*) malloc( buckets_n * sizeof(uint16_t) );
  // clear buckets each loop
  memset(buckets, 0, buckets_n * sizeof(*buckets));

  if (log_level>1){
    char s[100];
    sprintf(s,"FFT of %d readings X times per second.", num_samples);
    Serial.println(s);
    sprintf(s,"The %d bins will go into %d buckets, with first bucket containing %d bins.", num_samples/2, buckets_n, group_n);
    Serial.println(s);
    Serial.print("buckets[start_bin:end_bin]:");
  }

  for( uint8_t i = 0; i < buckets_n; i++ ){
    uint16_t bin_start, bin_end; // boundaries of this summed bucket
    if( i == 0 ){ // first bucket is a special case
      bin_start = 1;
      bin_end   = group_n;
    } else {
      // 2^n == 1 << n
      bin_start = ( 1 << ( i - 1 ) ) * group_n + 1;
      bin_end   = ( 1 << ( i     ) ) * group_n;
      if( bin_end >= num_samples/2 ){
        bin_end = num_samples/2 - 1; // last bin for real FFT
      }
    }
    if (log_level>1){
      char s[100];
      sprintf(s," %d[%d:%d]", i, bin_start, bin_end );
      Serial.print(s);
    }
    for( uint16_t j = bin_start; j <= bin_end; j++ ){
      buckets[i] += out[j];
    }
  }

  if(log_level>0){
    Serial.print(" Buckets: ");
    for( uint8_t i = 0; i < buckets_n; i++ ){
      pad(buckets[i], 5);
      Serial.print(" ");
    }
    Serial.println();
  }

  return buckets;
}
