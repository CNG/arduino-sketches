// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define PIN 5

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(185, PIN);

uint8_t  mode   = 0, // Current animation effect
         offset = 0; // Position of spinny eyes
uint32_t color  = 0x00F9C8; // Start red
uint32_t prevTime;

void setup() {
  pixels.begin();
  pixels.setBrightness(255); // 1/3 brightness
  prevTime = millis();
}

void loop() {
  uint8_t  i;
  uint32_t t;

  switch(mode) {

   case 0: // Random sparks - just one LED on at a time!
    i = random(106);
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(10);
    pixels.setPixelColor(i, 0);
    break;
 
   case 1: // Spinny wheels (8 LEDs on at a time)
    for(i=0; i<106; i++) {
      uint32_t c = 0;
      if(((offset + i) & 7) < 2) c = color; // 4 pixels on...
      pixels.setPixelColor(   i, c); // First eye
      pixels.setPixelColor(1-i, c); // Second eye (flipped)
    }
    pixels.show();
    offset++;
    delay(50);
    break;
  }

  t = millis();
  if((t - prevTime) > 8000) {      // Every 8 seconds...
    mode++;                        // Next mode
    if(mode > 1) {                 // End of modes?
      mode = 0;                    // Start modes over
      if(!color) color = 0x00F9C8; // Reset to red
    }
    for(i=1; i<106; i++) pixels.setPixelColor(i, 1);
    prevTime = t;
  }
}

