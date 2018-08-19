#include "utilities.h"

char * padded(int number, byte width=3) {
  char buffer[width];
  byte currentMax = 10;
  for (byte i=1; i<width; i++) {
    if (number < currentMax) {
      return "0";
    }
    currentMax *= 10;
  }
  sprintf(buffer, "%d", number);
  return buffer;
}

byte sines[] = { // quadrant 4 of sine wave 0-255
   0,  0,  0,  0,  1,  1,  1,  2,  2,  3,  4,  5,  5,  6,  7,  9,
  10 ,11 ,12 ,14 ,15 ,17 ,18 ,20 ,21 ,23 ,25 ,27 ,29 ,31 ,33 ,35,
  37 ,40 ,42 ,44 ,47 ,49 ,52 ,54 ,57 ,59 ,62 ,65 ,67 ,70 ,73 ,76,
  79 ,82 ,85 ,88 ,90 ,93 ,97,100,103,106,109,112,115,118,121,124
};
byte sine(byte x) {
  if ( x >= 192 ) { return       sines[63 - (x - 192)]; }
  if ( x >= 128 ) { return 255 - sines[     (x - 128)]; }
  if ( x >=  64 ) { return 255 - sines[63 - (x -  64)]; }
  else            { return       sines[      x       ]; }
}
