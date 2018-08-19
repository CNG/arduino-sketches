/**
 * Lights.h - Library for lights
 * Created by Charlie Gorichanaz.
 * Adapted from code by Adafruit Industries.
 */
#ifndef Lights_h
#define Lights_h

#include <Adafruit_NeoMatrix.h>

extern volatile boolean button_pushed;

unsigned long
  Wheel(byte position),
  Color(byte r, byte g, byte b),
  Color(byte r, byte g, byte b, byte w);

class Lights {
  private:
    byte pin_brightness;
  public:
    word width, height, mirrored_width;
    Adafruit_NeoMatrix lights;
    Lights(
      byte pin,
      word width_,
      word height_=0,
      byte matrix_type = NEO_MATRIX_TOP + NEO_MATRIX_RIGHT
        + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
      neoPixelType led_type = NEO_GRB + NEO_KHZ800
      )
      :
      width(width_),
      height(height_),
      mirrored_width(width / 2),
      lights(width, height, pin, matrix_type, led_type)
      {
        lights.begin();
      }
    void setBrightnessPin(byte pin);
    byte setBrightness(int level=-1);
    void show();
    void setMirroredPixelColor(word pixel, unsigned long color, char skew=1);
    void setMirroredPixelColor(word pixel, byte r, byte g, byte b, char skew=1);
    void setPixelColor(word pixel, unsigned long color, char skew=1);
    void setPixelColor(word pixel, unsigned long color, boolean mirrored, char skew=1);
    void setPixelColor(word pixel, byte r, byte g, byte b, char skew=1);
    void setPixelColor(word pixel, byte r, byte g, byte b, boolean mirrored, char skew=1);

    void rainbow(
      word end_pixel,
      word start_pixel = 0,
      word delay_ms = 0,
      word cycles = 256,
      boolean mirrored = false,
      boolean distributed = false
      );
    void colorWipe(
      unsigned long color,
      word end_pixel,
      word start_pixel = 0,
      word delay_ms = 0,
      boolean mirrored = false
      );
    void flash(
      word end_pixel = NULL,
      word start_pixel = 0,
      word delay_ms = 10,
      unsigned long start_color = Adafruit_NeoMatrix::Color(255, 255, 255),
      unsigned long end_color = Adafruit_NeoMatrix::Color(0, 0, 0)
      );
    void pulse(unsigned long c, byte brightness);
    void pulse2(unsigned long c, byte brightness);
    byte raincrazy(
      byte mode,
      byte delay_ms,
      byte spaces,
      boolean direction,
      boolean solid,
      boolean storm,
      boolean mirrored
      );
    byte thing(
      byte delay_ms,
      byte interval,
      word cycles,
      char skew,
      boolean direction,
      boolean mirrored
      );

};

#endif
