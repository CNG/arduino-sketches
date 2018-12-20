#include <Adafruit_NeoPixel.h>
#define PIN 5
Adafruit_NeoPixel strip = Adafruit_NeoPixel(185, PIN, NEO_GRB + NEO_KHZ800);

int switchPin = 10; // switch is connected to pin 10
int pinVal; // variable for reading the pin status
int pinVal2;
int buttonState; // variable to hold the button state
int lightMode = 0; // how many times the button has been pressed

void setup() {
  pinMode(10, INPUT_PULLUP); // Set the switch pin as input
  pinMode(6, OUTPUT); // not sure why this is here
  buttonState = digitalRead(switchPin); // read the initial state
  strip.begin();
  //strip.setBrightness(30);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {  
  lightMode = 3;
  if (lightMode == 0) {
    colorWipe(strip.Color(0, 0, 0), 0);

    delay(5000);
    pulse2(strip.Color(200,0,0));
    delay(5000);
    pulse2(strip.Color(0,200,0));
    delay(5000);
    pulse2(strip.Color(0,0,200));
  } else if (lightMode == 1) {
    
    pulse(strip.Color(255,0,0));
    pulse(strip.Color(128,128,0));
    pulse(strip.Color(0,255,0));
    pulse(strip.Color(0,128,128));
    pulse(strip.Color(0,0,255));
    pulse(strip.Color(128,0,128));
    pulse(strip.Color(125,125,125));
    //whiteFlash(60);

  } else if (lightMode == 2) {
    strip.setBrightness(128);
    rainbow(1);
    //rainbowCycle(1);
  } else if (lightMode == 3) {
    strip.setBrightness(160);
    for(uint16_t i=0; i<10; i++) {
      /*
      int times[] = {1,5,2,10};
      int spaces[] = {0,2,3,3,3,3,4,6,10,15,20};
      for(uint16_t j=0; j<sizeof(times); j++) {
        uint8_t wait = times[random(0,sizeof(times))];
        rainbow( 5, wait, spaces[random(0,sizeof(spaces))], j%2==0, random(0,15) == 0, random(0,30) == 0 );
      }
      */
      uint8_t times[]  = {  0, 1, 1, 2, 2, 3, 4, 6, 8,13 };
      //int spaces[] = {  0, 2, 2, 4, 6,10,16,26,42 };
      uint8_t spaces[] = { 15,10, 8, 7, 5, 5, 4, 3, 3, 0 };
      for(uint16_t j=0; j<sizeof(times); j++) {
        uint8_t wait   = times[j];
        uint8_t space  = spaces[j];
        rainbow2( 5, wait, space, j%2==0, random(0,8) == 0 && space > 0, random(0,20) == 0 );
      }
    }
  } else if (lightMode == 4) {
  } else if (lightMode == 5) {
  } else {
  }
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    if ( wait ) {
      strip.show();
      delay(wait);
    }
  }
  if ( wait == 0 ) {
    strip.show();
  }
}

void whiteFlash(uint8_t w) {

  for(uint16_t i=0; i<180; i++) {
      strip.setPixelColor(i, strip.Color(255, 255, 255));
  }
  strip.show();
  delay(w);
  for(uint16_t i=0; i<180; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  delay(w);


}


void rainbow2(uint8_t mode, uint8_t wait, uint8_t spaces, boolean dir, boolean solid, boolean storm) {
  uint16_t color, pixel;
  for(color=0; color<256; color++) {

    if( storm ) {
      if( color % 50 == 0 ) {
        uint8_t flashes = random(1,9);
        for( uint8_t p = 0; p<=flashes; p++ ){
          whiteFlash(60);
        }
        color = color + flashes;
        continue;
      }
    }

    for(pixel=0; pixel<180; pixel++) {
      if(mode==1||mode==2){
        // every pixel different color
        strip.setPixelColor(pixel, Wheel((pixel+color) & 255));
      }else if(mode==3||mode==4){
        // every pixel same color
        strip.setPixelColor(pixel, Wheel(color));
      }else if(mode==5||mode==6){
  
        uint8_t chunkSize = spaces + 1;
        // need to loop through num spaces but only trigger on one pixel
        for( uint8_t space = 0; space<=spaces; space++ ){
          if( pixel % chunkSize == space ){
            if( dir ){
              if( color % chunkSize == space ) {
                if( solid ){
                  uint16_t approx, colorDist;
                  colorDist = 255 / 6;
                  approx = color - (color % colorDist);
                  strip.setPixelColor(pixel, Wheel(approx));
                } else {
                  strip.setPixelColor(pixel, Wheel(color));
                }
              } else {
                strip.setPixelColor(pixel, strip.Color(0, 0, 0));
              }
            } else {
              if( color % chunkSize == ( spaces - space) ) {
                if( solid ){
                  uint16_t approx, colorDist;
                  colorDist = 255 / 6;
                  approx = color - (color % colorDist);
                  strip.setPixelColor(pixel, Wheel(approx));
                } else {
                  strip.setPixelColor(pixel, Wheel(color));
                }
              } else {
                strip.setPixelColor(pixel, strip.Color(0, 0, 0));
              }
            }
          }
        }
  
      }
    }
    strip.show();
    delay(wait);
    if(mode==2||mode==4||mode==6){
      for(uint8_t i=0; i<180; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
      strip.show();
      delay(wait);
    }  }
}

void pulse(uint32_t c) {
  for(uint16_t j=1; j<42; j++) {
    strip.setBrightness(j*6-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
  }
  for(uint16_t j=42; j>0; j--) {
    strip.setBrightness(j*6-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
  }
  strip.setBrightness(255);
}
void pulse2(uint32_t c) {
  for(uint16_t j=1; j<128; j++) {
    strip.setBrightness(j*2-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
  }
  delay(1000);
  for(uint16_t j=64; j>0; j--) {
    strip.setBrightness(j*4-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
  }
  strip.setBrightness(255);
}

//Rainbow Program
void rainbow(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<255; j++) {
    for(i=0; i<180; i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Rainbow Cycle Program - Equally distributed
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

