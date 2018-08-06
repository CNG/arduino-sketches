/*
 * This is rough beat detection based on frequency and amplitude but not using
 * FFT. It is a work in progress. I plan to use it as a base and add some averaging
 * to heal skipped beats, and have other visualizations during uncertain beat
 * periods. Video of this sketch is at https://vimeo.com/230314016
 *
 * generalized wave freq detection with 38.5kHz sampling rate and interrupts
 * by Amanda Ghassaei, Sept 2012
 * https: // www.instructables.com/id/Arduino-Frequency-Detection/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN 6
#define NUM_LEDS 60
#define BRIGHTNESS 50
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

 // clipping indicator variables
boolean clipping = 0;

 // data storage variables
byte newData = 0;
byte prevData = 0;
unsigned int time = 0; // keeps time and sends vales to store in timer[] occasionally
int timer[10]; // sstorage for timing of events
int slope[10]; // storage for slope of events
unsigned int totalTimer; // used to calculate period
unsigned int period; // storage for period of wave
byte index = 0; // current storage index
float frequency; // storage for frequency calculations
int maxSlope = 0; // used to calculate max slope as trigger point
int newSlope; // storage for incoming slope data

 // variables for decided whether you have a match
byte noMatch = 0; // counts how many non-matches you've received to reset variables if it's been too long
byte slopeTol = 30; // slope tolerance- adjust this if you need
int timerTol = 10; // timer tolerance- adjust this if you need

 // variables for amp detection
unsigned int ampTimer = 0;
byte maxAmp = 0;
byte checkMaxAmp;
byte ampThreshold = 18; // raise if you have a very noisy signal

void setup(){
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(115200);
//Serial.println(analogRead(A1));Serial.println(analogRead(A2));
  //pinMode(13,OUTPUT); // led indicator pin

  // set up continuous sampling of analog pin 0 at 38.5kHz
  cli(); // disable interrupts
  ADCSRA = 0; // clear ADCSRA and ADCSRB registers
  ADCSRB = 0;

  // Feather A0 is ADC7 https://cdn-learn.adafruit.com/assets/assets/000/041/528/original/microcomputers_Adafruit_Feather_32u4_Basic_Proto_v2_2.png?1494120696
  ADMUX=0x07; //Binary equivalent of 0111 http://www.robotplatform.com/knowledge/ADC/adc_tutorial_4.html

  ADMUX |= (1 << REFS0); // set reference voltage
  ADMUX |= (1 << ADLAR); // left align the ADC value- so we can read highest 8 bits from ADCH register only
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); // set ADC clock with 32 prescaler- 16mHz/32=500kHz
  ADCSRA |= (1 << ADATE); // enabble auto trigger
  ADCSRA |= (1 << ADIE); // enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN); // enable ADC
  ADCSRA |= (1 << ADSC); // start ADC measurements
  sei(); // enable interrupts
}

ISR(ADC_vect) { // when new ADC value ready

  prevData = newData; // store previous value
  newData = ADCH; // get value from A0
  if (prevData < 127 && newData >=127){ // if increasing and crossing midpoint
    newSlope = newData - prevData; // calculate slope
    if (abs(newSlope-maxSlope)<slopeTol){ // if slopes are ==
      // record new data and reset time
      slope[index] = newSlope;
      timer[index] = time;
      time = 0;
      if (index == 0){ // new max slope just reset
        noMatch = 0;
        index++; // increment index
      }
      else if (abs(timer[0]-timer[index])<timerTol && abs(slope[0]-newSlope)<slopeTol){ // if timer duration and slopes match
        // sum timer values
        totalTimer = 0;
        for (byte i=0;i<index;i++){
          totalTimer+=timer[i];
        }
        period = totalTimer; // set period
        // reset new zero index values to compare with
        timer[0] = timer[index];
        slope[0] = slope[index];
        index = 1; // set index to 1
        noMatch = 0;
      }
      else{ // crossing midpoint but not match
        index++; // increment index
        if (index > 9){
          reset();
        }
      }
    }
    else if (newSlope>maxSlope){ // if new slope is much larger than max slope
      maxSlope = newSlope;
      time = 0; // reset clock
      noMatch = 0;
      index = 0; // reset index
    }
    else{ // slope not steep enough
      noMatch++; // increment no match counter
      if (noMatch>9){
        reset();
      }
    }
  }

  if (newData == 0 || newData == 1023){ // if clipping
    PORTB |= B00100000; // turn on led 13
    clipping = 1; // currently clipping
  }

  time++; // increment timer at rate of 38.5kHz

  ampTimer++; // increment amplitude timer
  if (abs(127-ADCH)>maxAmp){
    maxAmp = abs(127-ADCH);
  }
  if (ampTimer==1000){
    ampTimer = 0;
    checkMaxAmp = maxAmp;
    maxAmp = 0;
  }

}

void reset(){ // clear out some variables
  index = 0; // reset index
  noMatch = 0; // reset match couner
  maxSlope = 0; // reset slope
}

void checkClipping(){ // manage clipping indicator LED
  if (clipping){ // if currently clipping
    PORTB &= B11011111; // turn off led 13
    clipping = 0;
  }
}

byte gravity_gamma[] = { 1, 1, 2, 3, 5, 8, 13, 21 };
void beat(){
  uint32_t c = strip.Color(255,0,255);
  int d = 0;
  int i,j;
  for(i = 0; i<NUM_LEDS; i++){
  for(j = 0; j<10; j++){
    strip.setPixelColor(i, c); 
  }
  strip.show(); delay(gravity_gamma[byte(i*8/NUM_LEDS)]*d);
  }
  for(i = NUM_LEDS-1; i>=0; i--){
  for(j = 0; j<10; j++){
    strip.setPixelColor(i, 0);
  }
    strip.show(); delay(gravity_gamma[byte(i*8/NUM_LEDS)]*d);
  }
  // delay(gravity_gamma[7]*d*3);
  delay(150);
}

void loop(){
  //checkClipping();
  if (checkMaxAmp>ampThreshold){
    frequency = 38462/float(period); // calculate frequency timer rate/period
    if (frequency<1800){
      // print results
      beat();
      Serial.print(millis());
      Serial.print(" ");
      Serial.print(checkMaxAmp);
      Serial.print(" ");
      Serial.print(frequency);
      Serial.println(" hz");
    }
  }
}
