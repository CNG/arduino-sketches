/*
  Notes on songs to test:

  every other beat is doubled: Human -- The Human League
  alternating drum and symbol: Candy -- Paolo Nutini
  deep long oscillations and alt double: If I Had A Heart -- Fever Ray
  weird beat, weird instruments: Nightwalking Your Memory -- Vector Lovers
    ^-- this one works pretty well with simple amplitude

 */

/*
  Smoothing can be done by adjusting `samples_inst_n` or 
 */


// main sample history container
const uint16_t samples_long_n = 220;
uint16_t samples_long[6][samples_long_n];
uint16_t samples_long_count = 0; // for rollover

// "instant" sample history container
const uint16_t samples_inst_n = 3;
uint16_t samples_inst[6][samples_inst_n];
uint16_t samples_inst_count = 0; // for rollover

uint16_t averages[6];
uint16_t variances[6];
uint16_t threshold = 20; // don't count beat if sample less than this
bool beats[6];
float ratios[6];


bool detect_beat(void* param, uint8_t log_level){
  uint16_t *bucket;
  bucket=(uint16_t*) param;

  // compute average of samples for each bucket
  for( uint16_t i = 0; i < 6; i++ ){
    samples_long[i][samples_long_count] = bucket[i]; // save sample for averaging
    samples_inst[i][samples_inst_count] = bucket[i]; // save sample for averaging
    int average = 0;
    for( uint16_t j = 0; j < samples_long_n; j++ ){
      average += samples_long[i][j];
    } 

    char s[100];
    sprintf(s," [ %05d(%03d) == %05d ]", samples_long[i][samples_long_count], samples_long_count, bucket[i] );
    SerialUSB.print(s);

    averages[i] = floor( average / samples_long_n );
  }

  if(log_level==2){
    char s[100];
    sprintf(s," Averages: %05d %05d %05d %05d %05d %05d", averages[0], averages[1], averages[2], averages[3], averages[4], averages[5]);
    SerialUSB.print(s);
  }

  // compute variance of samples for each bucket
  for( uint16_t i = 0; i < 6; i++ ){
    int sum_of_squared_diffs = 0;
    for( uint16_t j = 0; j < samples_long_n; j++ ){
      int diff = samples_long[i][j] - averages[i];
      sum_of_squared_diffs += diff * diff;
    }
    variances[i] = floor( sqrt( sum_of_squared_diffs ) / samples_long_n );
  }

  if(log_level==2){
    char s[100];
    sprintf(s," Variance: %04d %04d %04d %04d %04d %04d", variances[0], variances[1], variances[2], variances[3], variances[4], variances[5]);
    SerialUSB.print(s);
  }

  // compute whether current sample is higher than average by factor dep on variance
  for(int i = 0; i < 6; i++ ){

    int average = 0;
    for( uint16_t j = 0; j < samples_inst_n; j++ ){
      average += samples_inst[i][j];
    } 
    int now_sample = floor( average / samples_inst_n );

    if( now_sample < threshold ){
      ratios[i] = 0.00;
      beats[i]  = false;
    } else {

      if( averages[i] > 0){
        float ratio = (float) now_sample / averages[i];
        if( ratio > 9.99 ){
          ratios[i] = 9.99;
        } else {
          ratios[i] = ratio;
        }
        beats[i] = ratio > 1.0;
      } else {
        if( now_sample > 0){
          ratios[i] = 9.99;
          beats[i]  = true;
        } else {
          ratios[i] = 0.00;
          beats[i]  = false;
        }
      }

    }
  }

  if(log_level==2){
    for( uint16_t i = 0; i < 6; i++ ){
      if( beats[i] ){
        char s[4];
        sprintf(s," %03d", (int) (ratios[i] * 100.0) );
        SerialUSB.print(s);
        // SerialUSB.print(" ");
        // SerialUSB.print(ratios[i]);
        // SerialUSB.print(" ");
      } else {
        SerialUSB.print("    ");
      }
    }
  }

  if(log_level==2){
    // see which of 6 has highest variance
    int highest_variance =0, highest_slot=0;
    for( uint16_t i = 0; i < 6; i++ ){
      if( variances[i] > highest_variance ){
        highest_variance = variances[i];
        highest_slot = i;
      }
    }
    SerialUSB.print(" ");
    SerialUSB.print(highest_slot);  
    // // see if current sample for highest varying slot is above average
    // float ratio = samples_long[highest_slot][samples_long_count] / averages[highest_slot];
  }


  // advance sample counters
  if(++samples_long_count >= samples_long_n) samples_long_count = 0;
  if(++samples_inst_count >= samples_inst_n) samples_inst_count = 0;

}