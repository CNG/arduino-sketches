#ifndef FHT_SETUP_H
#define FHT_SETUP_H

#define FFT_N 512
#define ARM_MATH_CM3
#include <arm_math.h>

extern void pad( int number, byte width );

static int samprate = 12800;
arm_cfft_radix4_instance_q15 C;
arm_rfft_instance_q15 S;
volatile int bufn,obufn;
//static int bufn_max = FFT_N/128+1;
uint16_t buf[FFT_N/128+1][128];
q15_t out[FFT_N+128];
q15_t outsq[(FFT_N+128)*2];

// can't figure out how to move these to fht_setup.cpp
// "ADC was not declared in this scope"
void ADC_Handler(){
  int f=ADC->ADC_ISR;
  if (f&(1<<27)){
    bufn=(bufn+1);if(bufn==FFT_N/128+1)bufn=0;
    ADC->ADC_RNPR=(uint32_t)buf[bufn];
    ADC->ADC_RNCR=128;
  }
}

void fft_setup(){
  pmc_enable_periph_clk(ID_ADC);
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST);

  // http://asf.atmel.com/docs/3.1.3/sam.drivers.adc.adc_temp_sensor_example.sam4s_ek/html/component__adc_8h_source.html
  ADC->ADC_MR |=0x3; // Mode Register
  ADC->ADC_CHER=0x80; // Channel Enable Register
  // Nested Vector Interrupt Controller (NVIC)
  // http://www.keil.com/pack/doc/cmsis/Core/html/group___n_v_i_c__gr.html
  NVIC_EnableIRQ(ADC_IRQn); // enables the specified device-specific interrupt IRQn.
  ADC->ADC_IDR=~(1<<27); // Interrupt Disable Register: ADC_IDR_ENDRX (0x1u << 27) 
  ADC->ADC_IER=1<<27; // Interrupt Enable Register: ADC_IER_ENDRX (0x1u << 27)
  ADC->ADC_RPR=(uint32_t)buf[0]; // Receive Pointer Register
  ADC->ADC_RCR=128; // Receive Counter Register
  ADC->ADC_RNPR=(uint32_t)buf[1]; // Receive Next Pointer Register
  ADC->ADC_RNCR=128; // Receive Next Counter Register
  bufn=obufn=1;
  ADC->ADC_PTCR=1; // Transfer Control Register: ADC_PTCR_RXTEN (0x1u << 0)
  ADC->ADC_CR=2; // Control Register: ADC_CR_START (0x1u << 1)
  // Sets the priority for the interrupt specified by IRQn.
  // Lower values indicate a higher priority. The default priority is 0 for every interrupt. This is the highest possible priority.
  // For Cortex-M3, Cortex-M4, and Cortex-M7:
  // Supports 0 to 255 priority levels.
  NVIC_SetPriority(ADC_IRQn,6);

  pinMode(2,OUTPUT);    // port B pin 25  
  analogWrite(2,255);   // sets up some other registers I haven't worked out yet
  // PIO Disable Register
  REG_PIOB_PDR = 1<<25; // disable PIO, enable peripheral
  // (AB Select Status Register) determines whether the I/O pins are controlled by the PIO controller or the the corresponding on-chip peripheral.
  REG_PIOB_ABSR= 1<<25; // select peripheral B

  // ~/Library/Arduino15/packages/arduino/hardware/sam/1.6.6/system/CMSIS/Device/ATMEL/sam3n/html/TC0.html
  // TC0 Channel Mode Register (channel = 0):
  // 19-18: RB Loading Edge Selection: Falling edge of TIOA
  // 17-16: RA Loading Edge Selection: Rising edge of TIOA
  // 15:    Waveform Mode: Capture Mode is enabled
  // 14:    RC Compare Trigger Enable: RC Compare has no effect on the counter and its clock
  // 10:    TIOA or TIOB External Trigger Selection: TIOA is used as an external trigger
  // 9-8:   External Trigger Edge Selection: NONE, The clock is not gated by an external signal.
  // 7:     Counter Clock Disable with RB Loading: counter clock is not disabled when RB loading occurs
  // 6:     Counter Clock Stopped with RB Loading: counter clock is not stopped when RB loading occurs
  // 5-4:   Burst Signal Selection: The clock is not gated by an external signal
  // 3:     Clock Invert: counter is incremented on rising edge of the clock
  // 2-0:   Clock Selection: Clock selected: TIMER_CLOCK1, TCLK1
  REG_TC0_CMR0=0b00000000000010011100010000000000;
  // Register C (channel = 0)
  REG_TC0_RC0=42000000/samprate;
  // Register A (channel = 0)
  REG_TC0_RA0=1; 
  // TC0 Channel Control Register (channel = 0): enable counter clock; no effect; software trigger
  REG_TC0_CCR0=0b101;

  arm_rfft_init_q15(&S,&C,FFT_N,false,true);
}

void fft_loop(){
  while(obufn==bufn); // wait for buffer to be full
  obufn=bufn;

  int bb=bufn+1;if(bb==FFT_N/128+1)bb=0;
  uint16_t *q=buf[0]+128*bb;
  for(int j=0;j<FFT_N;j++){
    int t=(*q++)-2048;
    if(q==(buf[0]+FFT_N+128))q=buf[0];
    out[j]=t<<4;
  }
  arm_rfft_q15(&S,out,outsq);
  arm_cmplx_mag_squared_q15(outsq,out,FFT_N/2);
}

void* get_bins(){
  return (void*)out;
}

void fft_print_bins(){
  //SerialUSB.print(millis());
  for( int i=1;i<FFT_N/2; i++){
    //pad(out[i], 5);
    SerialUSB.print(out[i]);
    if( i == FFT_N/2 - 1 ){
      SerialUSB.println();
    } else {
      SerialUSB.print(" ");
    }
  }
}

// convert FFT_N/2 bins to $buckets summed buckets as in [Scheirer 1998]
// buckets will be like 200Hz, 400, 800, 1600, 3200, 6400
void* fft_buckets(uint8_t buckets, uint8_t log_level){
  uint16_t group_n = FFT_N/2 / ( 1 << ( buckets - 1 ) ); // # bins in first bucket, 2^n == 1 << n
  uint16_t bucket[ buckets ];
  memset(bucket, 0, sizeof(bucket)); // clear buckets each loop
  // char s[100];
  // sprintf(s,"FFT of %d readings %d times per second.", FFT_N, REG_TC0_RC0);
  // SerialUSB.println(s);
  // sprintf(s,"The %d bins will go into %d buckets, with first bucket containing %d bins.", FFT_N/2, buckets, group_n);
  // SerialUSB.println(s);
  // SerialUSB.print("bucket[start_bin:end_bin]:");
  for( uint8_t i = 0; i < buckets; i++ ){
    uint16_t bin_start, bin_end; // boundaries of this summed bucket
    if( i == 0 ){ // first bucket is a special case
      bin_start = 1;
      bin_end   = group_n;
    } else {
      // 2^n == 1 << n
      bin_start = ( 1 << ( i - 1 ) ) * group_n + 1;
      bin_end   = ( 1 << ( i     ) ) * group_n;
      if( bin_end >= FFT_N/2 ){
        bin_end = FFT_N/2 - 1; // last bin for real FFT
      }
    }
    // sprintf(s," %d[%d:%d]", i, bin_start, bin_end );
    // SerialUSB.print(s);
    for( uint16_t j = bin_start; j <= bin_end; j++ ){
      bucket[i] += out[j];
    }
  }
  // SerialUSB.println();

  if(log_level>0){
    SerialUSB.print(" Buckets: ");
    for( uint8_t i = 0; i < buckets; i++ ){
      pad(bucket[i], 5);
      SerialUSB.print(" ");
    }
  }

  return (void*)bucket;

}


#endif