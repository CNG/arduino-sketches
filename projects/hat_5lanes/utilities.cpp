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
