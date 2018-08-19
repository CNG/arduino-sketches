/**
 * VolumeMeter.cpp - Library for volume meters
 * Created by Charlie Gorichanaz.
 * Adapted from code by Adafruit Industries.
 */
#include "Lights.h"

//#define DEBUG
#ifdef DEBUG
 #define PRINT(x) Serial.print (x)
 #define PRINTLN(x) Serial.println (x)
#else
 #define PRINT(x)
 #define PRINTLN(x)
#endif


// Fast integrer square root function
// Adapted from https://www.stm32duino.com/viewtopic.php?t=56#p8160
unsigned int sqrt32(unsigned long n) {
  unsigned int c = 0x8000;
  unsigned int g = 0x8000;
  while(true) {
    if(g*g > n) {
      g ^= c;
    }
    c >>= 1;
    if(c == 0) {
      return g;
    }
    g |= c;
  }
}


// Input values 0 to 255 to get color values that transition R->G->B. 0 and 255
// are the same color. This is based on Adafruit's Wheel() function, which used
// a linear map that resulted in brightness peaks at 0, 85 and 170. This version
// uses a quadratic map to make values approach 255 faster while leaving full
// red or green or blue untouched. For example, Wheel(42) is halfway between
// red and green. The linear function yielded (126, 129, 0), but this one yields
// (219, 221, 0). This function is based on the equation the circle centered at
// (255,0) with radius 255:  (x-255)^2 + (y-0)^2 = r^2
unsigned long Wheel(byte position) {
  byte R = 0, G = 0, B = 0;
  if (position < 85) {
    R = sqrt32((1530 - 9 * position) * position);
    G = sqrt32(65025 - 9 * position * position);
  } else if (position < 170) {
    position -= 85;
    R = sqrt32(65025 - 9 * position * position);
    B = sqrt32((1530 - 9 * position) * position);
  } else {
    position -= 170;
    G = sqrt32((1530 - 9 * position) * position);
    B = sqrt32(65025 - 9 * position * position);
  }
  return Adafruit_NeoMatrix::Color(R, G, B);
}


// // use a quadratic map to make values approach 255 faster while leaving full
// // red or green or blue untouched
// unsigned long color(byte r, byte g, byte b) {
//   return Adafruit_NeoMatrix::Color(
//     sqrt32((510 - r) * r),
//     sqrt32((510 - g) * g),
//     sqrt32((510 - b) * b)
//   );
// }


// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
// Written by Adafruit.
unsigned long WheelLinear(byte position) {
  if (position < 85) {
    return Adafruit_NeoMatrix::Color(position * 3, 255 - position * 3, 0);
  } else if (position < 170) {
    position -= 85;
    return Adafruit_NeoMatrix::Color(255 - position * 3, 0, position * 3);
  } else {
    position -= 170;
    return Adafruit_NeoMatrix::Color(0, position * 3, 255 - position * 3);
  }
}


// Convert separate R,G,B into packed 32-bit RGB color.
// Packed format is always RGB, regardless of LED strand color order.
// Written by Adafruit.
uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}


// Convert separate R,G,B,W into packed 32-bit WRGB color.
// Packed format is always WRGB, regardless of LED strand color order.
// Written by Adafruit.
uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}


void Lights::setBrightnessPin(byte pin) {
  pin_brightness = pin;
  setBrightness();
}


// Set brightness based on passed in or potentiometer value.
// level can be 0-255. analogRead on Feather is 0-1023 and is mapped down.
byte Lights::setBrightness(int level) {
  if (level == -1) {
    // convert analogRead's range of 0 to 1023 to brightness range 0 to 255
    // level = map(analogRead(pin_brightness), 0, 1023, 0, 255);
    level = analogRead(pin_brightness) / 4;
  }
  lights.setBrightness(level);
  return (byte) level;
}

void Lights::show() {
  lights.show();
}

void Lights::setMirroredPixelColor(word pixel, unsigned long color, char skew) {
  pixel = pixel % mirrored_width;  // allow wrapping
  for (byte i = 0; i<height; i++) {
    word skewed_pixel = pixel+(i*skew);
    if (skewed_pixel < mirrored_width) {
      lights.drawPixel(skewed_pixel,i,color);
      lights.drawPixel(width-1-skewed_pixel, i, color);
    } else {
      // Revisit, one of these isn't needed/right? or is it for odd widths?
      lights.drawPixel(skewed_pixel,i,color);
      lights.drawPixel(skewed_pixel-width, i, color);
      lights.drawPixel(width-1-skewed_pixel, i, color);
    }
  }
}

void Lights::setMirroredPixelColor(word pixel, byte r, byte g, byte b, char skew) {
  setMirroredPixelColor(pixel, lights.Color(r, g, b), skew);
}

void Lights::setPixelColor(word pixel, unsigned long color, char skew) {
  pixel = pixel % width;  // allow wrapping
  for (byte i = 0; i<height; i++) {
    lights.drawPixel((pixel+(i*skew)) % width, i, color);
    // if (skewed_pixel < width) {
    //   lights.drawPixel(skewed_pixel, i, color);
    // } else {
    //   lights.drawPixel(skewed_pixel-width, i, color);
    // }
  }
}

void Lights::setPixelColor(word pixel, unsigned long color, boolean mirrored, char skew) {
  if (mirrored) {
    setMirroredPixelColor(pixel, color, skew);
  } else {
    setPixelColor(pixel, color, skew);
  }
}

void Lights::setPixelColor(word pixel, byte r, byte g, byte b, char skew) {
  setPixelColor(pixel, lights.Color(r, g, b), skew);
}

void Lights::setPixelColor(word pixel, byte r, byte g, byte b, boolean mirrored, char skew) {
  if (mirrored) {
    setMirroredPixelColor(pixel, lights.Color(r, g, b), skew);
  } else {
    setPixelColor(pixel, lights.Color(r, g, b), skew);
  }
}


// Fill the dots one after the other with a color
void Lights::colorWipe(
  unsigned long color,
  word end_pixel,
  word start_pixel,
  word delay_ms,
  boolean mirrored
) {
  for (word pixel = start_pixel; pixel <= end_pixel; pixel++) {
    setPixelColor(pixel, color, mirrored);
    show();
    delay(delay_ms);
  }
}


void Lights::rainbow(
  word end_pixel,
  word start_pixel,
  word delay_ms,
  word cycles,
  boolean mirrored,
  boolean distributed
) {
  static byte cycle = 0;
  static word factor_10 = 256 * 10 / cycles;
  static unsigned long color;
  for (word pixel = start_pixel; pixel <= end_pixel; pixel++) {
    if (distributed) {
      color = Wheel(((pixel*256/width) + (cycle*factor_10/10)) & 255);
    } else {
      color = Wheel((pixel + (cycle*factor_10/10)) & 255);
    }
    setPixelColor(pixel, color, mirrored);
  }
  delay(delay_ms);
  if (++cycle == cycles) { cycle = 0; }
}


void Lights::flash(
  word end_pixel,
  word start_pixel,
  word delay_ms,
  unsigned long start_color,
  unsigned long end_color
) {
  if (end_pixel == NULL) {
    end_pixel = width;  // Can I do this in the header?
  }
  for (word pixel = start_pixel; pixel < end_pixel; pixel++) {
    setPixelColor(pixel, start_color);
  }
  show();
  delay(delay_ms);
  for (word pixel = start_pixel; pixel < end_pixel; pixel++) {
    setPixelColor(pixel, end_color);
  }
  show();
  delay(delay_ms);
}


void Lights::pulse(unsigned long color, byte brightness) {
  for (word step = 1; step < 42; step++) {
    if (button_pushed) { return; }
    lights.setBrightness(map(step*6-1,0,255,0,brightness));
    for (word pixel = 0; pixel < width; pixel++) {
      setPixelColor(pixel, color);
    }
    show();
  }
  for (word step = 42; step > 0; step--) {
    if (button_pushed) { return; }
    lights.setBrightness(map(step*6-1,0,255,0,brightness));
    for (word pixel = 0; pixel < width; pixel++) {
      setPixelColor(pixel, color);
    }
    show();
  }
  lights.setBrightness(brightness);
}


void Lights::pulse2(unsigned long color, byte brightness) {
  for (word step = 1; step < 128; step++) {
    if (button_pushed) { return; }
    lights.setBrightness(map(step*2-1,0,255,0,brightness));
    for (word pixel = 0; pixel < width; pixel++) {
      setPixelColor(pixel, color);
    }
    show();
  }
  delay(1000);
  for (word step = 64; step > 0; step--) {
    if (button_pushed) { return; }
    lights.setBrightness(map(step*4-1,0,255,0,brightness));
    for (word pixel = 0; pixel < width; pixel++) {
      setPixelColor(pixel, color);
    }
    show();
  }
  lights.setBrightness(brightness);
}


byte Lights::thing(
  byte delay_ms,
  byte interval,
  word cycles,
  char skew,
  boolean direction,
  boolean mirrored
) {
  static bool last_direction = direction;
  static word
    pixels = mirrored ? mirrored_width : width,
    pixel;
  static int
    cycle = direction ? 0 : cycles;
  if (direction != last_direction) {
    cycle = direction ? 0 : cycles;
    last_direction = direction;
  }

  // for (pixel=0; pixel < pixels; pixel++) {
  //   setPixelColor(pixel, Wheel((128 + cycle + pixel * 256 / pixels) & 255), mirrored, 0);
  // }

  //byte interval = 16;
  for (pixel=0; pixel < pixels; pixel++) {
    PRINT(pixel); PRINT(" ");
    PRINT(interval); PRINT(" ");
    PRINT(pixel % interval); PRINT(" ");
    PRINT(interval - (pixel % interval)); PRINT(" ");
    if (pixel % interval == 0) {
      // full power!
      setPixelColor(pixel+cycle, lights.Color(255, 0, 0), mirrored, skew);
    } else if (interval > 2 && pixel % interval == 1 || interval - (pixel % interval) == 1) {
      // so close!
      setPixelColor(pixel+cycle, lights.Color(163, 0, 0), mirrored, skew);
    } else if (interval > 4 && pixel % interval == 2 || interval - (pixel % interval) == 2) {
      // almost!
      setPixelColor(pixel+cycle, lights.Color(92, 0, 0), mirrored, skew);
    } else {
      // the space between
      setPixelColor(pixel+cycle, lights.Color(0, 0, 0), mirrored, skew);
    }
    PRINTLN(" ");

  }

  show();
  delay(delay_ms);
  if (direction) {
    if (++cycle == cycles) { cycle = 0; }
  } else {
    if (--cycle == 0) { cycle = cycles; }
  }
  // must return 0 at last cycle
  return direction ? cycle : cycles - cycle;
}


byte Lights::raincrazy(
  byte mode,
  byte delay_ms,
  byte spaces,
  boolean direction,
  boolean solid,
  boolean storm,
  boolean mirrored
) {
  static word pixels = mirrored ? mirrored_width : width;
  //static word color = 0;
  word color;
  word pixel;
  for(color=0; color<256; color++) {
  //if (++color == 256) { color = 0; }
  if (storm) {
    if (color % 50 == 0) {
      byte flashes = random(1,9);
      for (byte p = 0; p<=flashes; p++) {
        flash(0, width, 60);
      }
      color = color + flashes;
      return 1;
    }
  }
  delay(7);
  for (pixel=0; pixel < pixels; pixel++) {
    if (mode <= 4) {
      // every pixel different color or every pixel different
      byte temp = mode < 3 ? (pixel+color) & 255 : color;
      setPixelColor(pixel, Wheel(temp), mirrored);
    } else {

      byte chunkSize = spaces + 1;
      // need to loop through num spaces but only trigger on one pixel
      for (byte space = 0; space <= spaces; space++) {
        if (pixel % chunkSize == space) {
          word approx, colorDist = 255 / 6;
          if (direction) {
            if (color % chunkSize == space) {

              approx = color;
              if (solid) {
                approx = color - (color % colorDist);
              }
              setPixelColor(pixel, Wheel(approx), mirrored);

            } else {
              setPixelColor(pixel, Color(0, 0, 0), mirrored);
            }
          } else {
            if (color % chunkSize == spaces - space) {

              approx = color;
              if (solid) {
                approx = color - (color % colorDist);
              }
              setPixelColor(pixel, Wheel(approx), mirrored);

            } else {
              setPixelColor(pixel, Color(0, 0, 0), mirrored);
            }
          }
        }
      }

    }
  }
  show();
  delay(delay_ms);
  if (mode==2 || mode==4 || mode==6) {
    for (word pixel = 0; pixel < width; pixel++) {
      setPixelColor(pixel, Color(0, 0, 0));
    }
    show();
    delay(delay_ms);
  }}
  return color;

}

