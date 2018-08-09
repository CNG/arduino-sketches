/**
 * VolumeMeter.cpp - Library for volume meters
 * Created by Charlie Gorichanaz.
 * Adapted from code by Adafruit Industries.
 */
#include "Lights.h"

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
// Written by Adafruit.
unsigned long Wheel(byte position) {
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

void Lights::setMirroredPixelColor(word pixel, unsigned long color) {
  for (byte i = 0; i<height; i++) {
    if (pixel+i < width) {
      lights.drawPixel(pixel+i,i,color);
      lights.drawPixel(width-pixel-1+i, i, color);
    } else {
      lights.drawPixel(pixel+i,i,color);
      lights.drawPixel(pixel+i-width, i, color);
      lights.drawPixel(width-pixel-1+i, i, color);
    }
  }
}

void Lights::setMirroredPixelColor(word pixel, byte r, byte g, byte b) {
  setMirroredPixelColor(pixel, lights.Color(r, g, b));
}

void Lights::setPixelColor(word pixel, unsigned long color) {
  for (byte i = 0; i<height; i++) {
    if (pixel+i < width) {
      lights.drawPixel(pixel+i, i, color);
    } else {
      lights.drawPixel(pixel+i-width, i, color);
    }
  }
}

void Lights::setPixelColor(word pixel, unsigned long color, boolean mirrored) {
  if (mirrored) {
    setMirroredPixelColor(pixel, color);
  } else {
    setPixelColor(pixel, color);
  }
}

void Lights::setPixelColor(word pixel, byte r, byte g, byte b) {
  setPixelColor(pixel, lights.Color(r, g, b));
}

void Lights::setPixelColor(word pixel, byte r, byte g, byte b, boolean mirrored) {
  if (mirrored) {
    setMirroredPixelColor(pixel, lights.Color(r, g, b));
  } else {
    setPixelColor(pixel, lights.Color(r, g, b));
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


void Lights::raincrazy(
  byte mode,
  byte delay_ms,
  byte spaces,
  boolean dir,
  boolean solid,
  boolean storm
) {
  // static word color = 0;
  word color;
for(color=0; color<256; color++) {
  word pixel;
  if (storm) {
    if (color % 50 == 0) {
      byte flashes = random(1,9);
      for (byte p = 0; p<=flashes; p++) {
        flash(0, width, 60);
      }
      color = color + flashes;
      continue;
      //return;
    }
  }
  delay(7);
  for (pixel=0; pixel < mirrored_width; pixel++) {
    if (mode==1 || mode==2) {
      // every pixel different color
      setMirroredPixelColor(pixel, Wheel((pixel+color) & 255));
    } else if (mode==3 || mode==4) {
      // every pixel same color
      setMirroredPixelColor(pixel, Wheel(color));
    } else if (mode==5 || mode==6) {

      byte chunkSize = spaces + 1;
      // need to loop through num spaces but only trigger on one pixel
      for (byte space = 0; space <= spaces; space++) {
        if (pixel % chunkSize == space) {
          if (dir) {
            if (color % chunkSize == space) {
              if (solid) {
                word approx, colorDist;
                colorDist = 255 / 6;
                approx = color - (color % colorDist);
                setMirroredPixelColor(pixel, Wheel(approx));
              } else {
                setMirroredPixelColor(pixel, Wheel(color));
              }
            } else {
              setMirroredPixelColor(pixel, Color(0, 0, 0));
            }
          } else {
            if (color % chunkSize == spaces - space) {
              if (solid) {
                word approx, colorDist;
                colorDist = 255 / 6;
                approx = color - (color % colorDist);
                setMirroredPixelColor(pixel, Wheel(approx));
              } else {
                setMirroredPixelColor(pixel, Wheel(color));
              }
            } else {
              setMirroredPixelColor(pixel, Color(0, 0, 0));
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
  }
}
//  if (++color == 256) { color = 0; }
}

