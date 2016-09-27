#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

/*
  TODO (this includes nonsettings)
  Hardware and connection dependent settings
 */

//int lightMode = -1; // one less than we want bc interrupt runs at beginning apparently
int lightMode = 1; // one less than we want bc interrupt runs at beginning apparently

#include "strip_functions.h"
int _num_led_cols = 2; // Number of columns of LEDs
int _num_led_rows = 150; // Number of pixels in each column
uint8_t _pin_led = 1; // NeoPixel LED strand is connected to this pin
uint8_t _pin_lev = A9; // using 1K potentiometer
uint8_t _pin_opt = A11; // using 1K potentiometer
extern boolean checkButton();

#include "sound_reactive.h"
uint8_t _pin_mic = A7;  // Microphone is attached D6
extern int vol;


/* Initialize variables */

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
  _num_led_rows, _num_led_cols, _pin_led,
  NEO_MATRIX_BOTTOM  + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix strip = matrix;

boolean pushed = false;
/**
 * Instead doing work directly within interrupt, we'll just toggle a
 * variable that we'll check for later. This prevents any weirdness due to 
 * variable volatility.
 */
void buttonInterrupt() {
  pushed = true;
}


void setup() {

  /* Interrupt setup */

    /*
      "FLORA supports external interrupts on D0 (INT2), D1(INT3), D2(INT1), and 
      D3(INT0). You would attach your button between, say, D0 and GND."
      FLORA's D0 is labeled "RX" (https://learn.adafruit.com/assets/2845)
      and corresponds to Arduino interrupt 2.
      * https://www.arduino.cc/en/Reference/PinMode
      * https://www.arduino.cc/en/Reference/attachInterrupt
      * https://forums.adafruit.com/viewtopic.php?f=25&t=57650:
     */
    pinMode(0, INPUT_PULLUP); // configure pin to behave as input
    // when button is pushed, run buttonPushed()
    attachInterrupt(2, buttonInterrupt, FALLING); // listen for high to low

    // from Amplitie code, not sure if needed or what it does
    //pinMode(_pin_led, OUTPUT);

    // http://www.avrfreaks.net/forum/timer1-atmega32u4-arduino-leonardo
    cli();          // disable global interrupts

    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
    // set compare match register to desired timer count
    //OCR1A = 781; // 10 times a second
    OCR1A = 78; // 100 times a second
    //OCR1A = 8; // 1000 times a second
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    TCCR3A = 0;     // set entire TCCR1A register to 0
    TCCR3B = 0;     // same for TCCR1B
    // set compare match register to desired timer count
    //OCR3A = 781; // 10 times a second
    OCR3A = 78; // 100 times a second
    //OCR3A = 8; // 1000 times a second
    // turn on CTC mode:
    TCCR3B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler
    TCCR3B |= (1 << CS10);
    TCCR3B |= (1 << CS12);
    // enable timer compare interrupt
    TIMSK3 |= (1 << OCIE1A);

    // enable global interrupts
    sei();
  
  /* sound_reactive */

    memset((void*) vol, 0, sizeof(vol));

    /*
      This is only needed on 5V Arduinos (Uno, Leonardo, etc.). Connect 3.3V to 
      mic AND TO AREF ON ARDUINO and enable this line.  Audio samples are 
      'cleaner' at 3.3V. COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.): 
     */
    // analogReference(EXTERNAL);

  matrix.begin();
  matrix.show(); // Initialize all pixels to 'off'

  //while (!Serial);
  //Serial.begin(9600);

}


volatile uint16_t opt = 0;
SIGNAL(TIMER1_COMPA_vect) {
  opt = analogRead(_pin_opt);
}
volatile bool flag = false;
SIGNAL(TIMER3_COMPA_vect) {
  flag = true;
}


void setVerticalPixelColor(uint16_t n, uint32_t c){
  for(uint8_t i = 0; i < _num_led_cols; i++) {
    matrix.drawPixel(n, i, c);
  }
}
void setHorizontalPixelColor(uint16_t n, uint32_t c){
  for(uint8_t i = 0; i < _num_led_rows; i++) {
    matrix.drawPixel(n, i, c);
  }
}
void setPixelColor(uint16_t n, uint32_t c){
  setVerticalPixelColor(n, c);
}
int numPixels(){
  return _num_led_rows;
}
void show(){
  matrix.show();
}
void setBrightness(uint8_t n){
  matrix.setBrightness(n);
}
uint16_t color(uint8_t r, uint8_t g, uint8_t b){
  matrix.Color(r, g, b);
}

void loop() {

  checkButton();

  //Serial.println( getOpt(_pin_lev, 0, 255));

  switch (lightMode) {
    default:
      // we exceeded defined modes, reset and fall through
      lightMode = 0;
    case 0:
      colorWipe(strip.Color(0, 0, 0), 0);
      delay(20);
      break;
    case 1:
      colorWipe(strip.Color(255, 255, 255), 0);
      delay(20);
      setBrightness();
      break;
    case 2: rainbowSingle(); break;
    case 3: rainbowCycle(); break;
    case 4: rainbowCycle2(); break;
    case 5:
      pulse(0);
      break;
    case 6:
      //volMeter(120,220);
      if( flag ){
        flag = false;
        volMeter(80,150);
      }
      break;
    case 7:
      //volMeter(120,220);
      if( flag ){
        flag = false;
        volMeter(120,220);
      }
      break;
    //case 5: volMeter(220,250); break;
    //case 6: volMeter( 20, 60); break;
    //case 7: volMeter(  1,255); break;
  }

}

