#include <Adafruit_NeoPixel.h>


/*
  TODO (this includes nonsettings)
  Hardware and connection dependent settings
 */

//int lightMode = -1; // one less than we want bc interrupt runs at beginning apparently
int lightMode = 4; // one less than we want bc interrupt runs at beginning apparently

#include "strip_functions.h"
uint16_t _num_led = 130; // Number of pixels in strand
uint8_t _pin_led = 6; // NeoPixel LED strand is connected to this pin
uint8_t _pin_lev = A9; // using 1K potentiometer
uint8_t _pin_opt = A11; // using 1K potentiometer
extern boolean checkButton();

#include "sound_reactive.h"
uint8_t _pin_mic = A10; // Microphone is attached to this analog pin
extern int vol;


/* Initialize variables */

Adafruit_NeoPixel
  strip = Adafruit_NeoPixel(_num_led, _pin_led, NEO_GRB + NEO_KHZ800);

boolean pushed = false;
/**
 * Instead doing work directly within interrupt, we'll just toggle a
 * variable that we'll check for later. This prevents any weirdness due to 
 * variable volatility.
 */
void buttonInterrupt() {
  pushed = true;
}

volatile uint16_t opt = 0;
void timerInterrupt() {
  opt = analogRead(_pin_opt);
}

//int lightMode = 0; // how many times the button has been pressed

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

    // https://learn.adafruit.com/multi-tasking-the-arduino-part-2/timers
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function below
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  
  /* sound_reactive */

    memset((void*) vol, 0, sizeof(vol));

    /*
      This is only needed on 5V Arduinos (Uno, Leonardo, etc.). Connect 3.3V to 
      mic AND TO AREF ON ARDUINO and enable this line.  Audio samples are 
      'cleaner' at 3.3V. COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.): 
     */
    // analogReference(EXTERNAL);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}


// Interrupt is called once a millisecond
SIGNAL(TIMER0_COMPA_vect) {
  timerInterrupt();
}

void loop() {

  checkButton();

  switch (lightMode) {
    default:
      // we exceeded defined modes, reset and fall through
      lightMode = 0;
    case 0:
      colorWipe(strip.Color(0, 0, 0), 0);
      delay(20);
      break;
    case 1: rainbowSingle(); break;
    case 2: rainbowCycle(); break;
    case 3:
      pulse(0);
      break;
    case 4: volMeterSimple(120,220); break;
    //case 5: volMeter(220,250); break;
    //case 6: volMeter( 20, 60); break;
    //case 7: volMeter(  1,255); break;
  }

}

