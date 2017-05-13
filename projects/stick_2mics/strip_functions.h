#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel strip;
extern boolean pushed;
extern volatile uint16_t opt;
extern uint8_t _pin_lev;
extern uint8_t _pin_opt;
extern int lightMode;

void buttonPushed();
void setBrightness();
int getOpt(uint8_t pin, int min, int max);

void colorWipe(uint32_t c, uint8_t wait);
void pulse(int w);
void rainbow();
void rainbowCycle();
void rainbowSingle();

uint32_t Wheel(byte WheelPos);
