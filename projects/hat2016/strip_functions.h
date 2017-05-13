#include <Adafruit_NeoPixel.h> // don't need functions but defines some types

extern bool pushed;
extern volatile uint16_t opt;
extern uint8_t _pin_lev;
extern uint8_t _pin_opt;
extern int lightMode;

extern void setVerticalPixelColor(uint16_t n, uint32_t c);
extern void setHorizontalPixelColor(uint16_t n, uint32_t c);
extern void setPixelColor(uint16_t n, uint32_t c);
extern int numPixels();
extern void show();
extern void setBrightness(uint8_t n);
extern uint16_t color(uint8_t r, uint8_t g, uint8_t b);

void buttonPushed();
void setBrightness();
int getOpt(uint8_t pin, int min, int max);

void colorWipe(uint32_t c, uint8_t wait);
void pulse(int w);
void rainbow();
void rainbowCycle();
void rainbowCycle2();
void rainbowSingle();

uint32_t Wheel(byte WheelPos);
uint32_t Wheel2(byte WheelPos);
