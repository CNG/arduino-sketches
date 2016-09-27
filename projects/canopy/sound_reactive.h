/*
LED VU meter for Arduino and Adafruit NeoPixel LEDs.

Hardware requirements:
 - Most Arduino or Arduino-compatible boards (ATmega 328P or better).
 - Adafruit Electret Microphone Amplifier (ID: 1063)
 - Adafruit Flora RGB Smart Pixels (ID: 1260)
   OR
 - Adafruit NeoPixel Digital LED strip (ID: 1138)
 - Optional: battery for portable use (else power through USB or adapter)
Software requirements:
 - Adafruit NeoPixel library

Connections:
 - 3.3V to mic amp +
 - GND to mic amp -
 - Analog pin to microphone output (configurable below)
 - Digital pin to LED data input (configurable below)
 See notes in setup() regarding 5V vs. 3.3V boards - there may be an
 extra connection to make and one line of code to enable or disable.

Written by Adafruit Industries.  Distributed under the BSD license.
This paragraph must be included in any redistribution.
*/
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoMatrix matrix, strip;
extern boolean checkButton();

extern void setVerticalPixelColor(uint16_t n, uint32_t c);
extern void setHorizontalPixelColor(uint16_t n, uint32_t c);
extern void setPixelColor(uint16_t n, uint32_t c);
extern int numPixels();
extern void show();
extern void setBrightness(uint8_t n);
extern uint16_t color(uint8_t r, uint8_t g, uint8_t b);

extern uint8_t  _pin_led;
extern uint8_t  _pin_mic;
extern uint8_t _pin_opt;

extern uint32_t Wheel(byte WheelPos);
extern void setBrightness();
extern int getOpt(uint8_t pin, int min, int max);

extern void setPixelColor(uint16_t n, uint32_t c);

void pad( int number, byte width );

void volMeter(uint8_t startColor, uint8_t endColor);
void setMeterPixelColor(uint16_t n, uint32_t c);
