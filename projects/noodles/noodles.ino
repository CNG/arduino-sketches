#include <Adafruit_NeoMatrix.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define LED_PIN 6
#define NUM_LEDS 75
#define NUM_COLS 6
#define BRIGHTNESS 255

//Adafruit_NeoPixel strip = Adafruit_NeoPixel(600, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(NUM_LEDS, NUM_COLS, LED_PIN, NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

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
byte slopeTol = 5; // slope tolerance- adjust this if you need
int timerTol = 5; // timer tolerance- adjust this if you need
// variables for amp detection
unsigned int ampTimer = 0;
byte maxAmp = 0;
byte checkMaxAmp;
byte ampThreshold = 22; // raise if you have a very noisy signal





uint16_t i, j, k;
bool ramp = true;
//bool debug = false;
bool debug = true;
byte c;
uint32_t color;

void setup() {
  if (debug) Serial.begin(115200);
  matrix.setBrightness(BRIGHTNESS);
  matrix.begin();
  matrix.show(); // Initialize all pixels to 'off'

  randomSeed(analogRead(A0));

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
byte beatmode = 0;
byte skip = 15;
void beat(bool big){
  uint32_t c = Wheel(random(0,255));
  int d = 0;
  int i,j;
  byte col = beatmode;
  if( beatmode > NUM_COLS ) col = random(0,NUM_COLS);

  if( big ){
    for(i = 0; i<NUM_LEDS; i++){
      for(j = 0; j<NUM_COLS; j++){ matrix.drawPixel(i, j, c); }
      if (i%skip==0||i==NUM_LEDS-1) matrix.show(); //delay(gravity_gamma[byte(i*8/NUM_LEDS)]*d);
    }
    delay(50);
    for(i = NUM_LEDS-1; i>=0; i--){
      for(j = 0; j<NUM_COLS; j++){ matrix.drawPixel(i, j, 0); }
      if (i%skip==0||i==0) matrix.show(); //delay(gravity_gamma[byte(i*8/NUM_LEDS)]*d);
    }
  }else{
    for(i = 0; i<NUM_LEDS; i++){
      matrix.drawPixel(i, col, c);
      if (i%skip==0||i==NUM_LEDS-1) matrix.show(); //delay(gravity_gamma[byte(i*8/NUM_LEDS)]*d);
    }
    delay(50);
    for(i = NUM_LEDS-1; i>=0; i--){
      matrix.drawPixel(i, col, 0);
      if (i%skip==0||i==0) matrix.show(); //delay(gravity_gamma[byte(i*8/NUM_LEDS)]*d);
    }
  }

  // delay(gravity_gamma[7]*d*3);
  //delay(150);

  beatmode++;
  if( beatmode == NUM_COLS * 3 ) beatmode = 0;
}

unsigned long currentMillis=0;
byte leading_smalls = 5;

byte checkbeat(){
  static int intervalSmall=10;
  static unsigned long previousMillisSmall=0;
  static int intervalBig=800;
  static unsigned long previousMillisBig=0;
  byte returnVal = 0;
  //checkClipping();
  if (checkMaxAmp>ampThreshold){
    frequency = 38462/float(period); // calculate frequency timer rate/period
    if (frequency<1800){
      currentMillis = millis();
      if ((unsigned long)(currentMillis - previousMillisSmall) >= intervalSmall) {
        if ((unsigned long)(currentMillis - previousMillisBig) >= intervalBig) {
          if( leading_smalls > 0 ){
            returnVal = 1;
            leading_smalls--;
          } else {
            returnVal = 2;
          }
          previousMillisBig = currentMillis;
        }else{
          returnVal = 1;
        }
        if (debug) {
          Serial.print(millis()); Serial.print(" ");
          Serial.print(checkMaxAmp); Serial.print(" ");
          Serial.print(frequency); Serial.println(" hz");
        }
        previousMillisSmall = currentMillis;
      }
    }
  }
  return returnVal;
}

void nobeat(){
  static int r = 0;

  //for(r=0; r<256*3; r++) {
    //for(j=0; j<NUM_LEDS; j=j+3) {
      for(i=0; i<NUM_LEDS; i++) {
        c = (i*256/NUM_LEDS +r) & 255;
        //if (debug) Serial.print(c);
        //if (debug) Serial.print(" ");
        color = Wheel(c);
        for(k=0; k<NUM_COLS; k++) {
          matrix.drawPixel(i, k, color);
        }
        if (ramp && i%5==0) matrix.show();
      }
      matrix.show();
      //if (debug) Serial.println(" ");
      ramp = false;
    //}
  //}

  if( r<256*3 ) r++;
  else r = 0;
}
void rampdown(){
  for(i=0; i<NUM_LEDS; i++) {
    for(k=0; k<NUM_COLS; k++) {
      matrix.drawPixel(i, k, 0);
    }
    if (i%5==0) matrix.show();
  }
}

void loop() {
  currentMillis = millis();
  switch( checkbeat() ){
    case 0:
      static int intervalBeat=400;
      static unsigned long previousMillisBeat=0;
      if ((unsigned long)(currentMillis - previousMillisBeat) >= intervalBeat) {
        nobeat();
        leading_smalls = 5;
      }
      break;
    case 1:
      //if (!ramp) rampdown(); // clear the board from last nobeat
      beat(false);
      previousMillisBeat = currentMillis;
      ramp = true; // make next nobeat ramp up from black
      break;
    case 2:
      if (!ramp) rampdown(); // clear the board from last nobeat
      beat(true);
      previousMillisBeat = currentMillis;
      ramp = true; // make next nobeat ramp up from black
      break;
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
