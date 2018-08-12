/*
started plugging Adafruit_ZeroFFT code into this to get FFT on 
M0 working, but i think I need more speed, so I switched to seeing 
if reading the datasheets could help me adapt the M3 code I was
using. so, this is functional but not fast
 */

#include "Adafruit_ZeroFFT.h"

#define DATA_SIZE 256

//the sample rate
#define FS 8000
byte log_level = 1;
int16_t data[DATA_SIZE];

void setup() {
  if( log_level ){
    Serial.begin(115200);
    while(!Serial); //wait for serial to be ready
  }
}


byte count = 0;
void loop() {
  if(log_level>0){ Serial.print(millis()); }

  long avg = 0;
  for(word i=0; i<DATA_SIZE; i++){
    int val = analogRead(A0);
    avg += val;
    data[i] = val;
  }

  //remove DC offset and gain up to 16 bits
  avg = avg/DATA_SIZE;
  for(int i=0; i<DATA_SIZE; i++) data[i] = (data[i] - avg) * 64;

  //run the FFT
  ZeroFFT(data, DATA_SIZE);

  //data is only meaningful up to sample rate/2, discard the other half
  for(int i=0; i<DATA_SIZE/2; i++){
    
    if(log_level>1){
      //print the frequency
      Serial.print(FFT_BIN(i, FS, DATA_SIZE));
      Serial.print(" Hz: ");

      //print the corresponding FFT output
      Serial.println(data[i]);
    }
  }

  // if( count == 0 ){ // too jittery if we do the lights every 1/100 second
  //   uint16_t * buckets = fft_buckets(6, log_level);
  //   led_show(buckets, log_level);

  //   //detect_beat(buckets, log_level);

  //   // uint16_t * bins = get_bins(1);
  //   // led_show_32(bins, log_level);
  // }

  if (log_level > 0) { Serial.println(); }
  if (++count >= 3) { count = 0; }
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
