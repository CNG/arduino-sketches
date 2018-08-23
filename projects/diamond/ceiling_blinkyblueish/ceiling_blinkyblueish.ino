#include <Adafruit_NeoPixel.h>
#include <math.h>

#define PIN 5
#define MIC_PIN   A5  // Microphone is attached to this analog pin
#define N_PIXELS  185  // Number of pixels in strand
#define SAMPLE_WINDOW   10  // Sample window for average level
#define PEAK_HANG 1 //Time of pause before peak dot falls
#define PEAK_FALL 4 //Rate of falling peak dot
#define INPUT_FLOOR 0 //Lower range of analogRead input
#define INPUT_CEILING 500//Max range of analogRead input, the lower the value the more sensitive (1023 = max)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_PIXELS, PIN, NEO_RGB);

// length of time
int PLAYA_LENGTH = 8000;
int BLINK_LENGTH = 4800;
int SPARKLE_LENGTH = 3200;
long currentMillis = 0;

//playa cloud vars
byte peak = 16;      // Peak level of column; used for falling dots
unsigned int sample;
byte dotCount = 0;  //Frame counter for peak dot
byte dotHangCount = 0; //Frame counter for holding peak dot



void setup() {
  strip.setBrightness(30);
  strip.begin();
  strip.show(); 
}

void loop() {
  rainbow(1);
  playacloud();
  sparkle(strip.Color(255,128,212));
  sparkle(strip.Color(255,255,255));
  rainbow2(1);
  playacloud();
}



////////////
// blinky //
////////////

void rainbow2(uint8_t wait) {
  uint16_t i, s, p;
  s = 0; 
  p = random(1,5);
  int counter = 0;
  currentMillis = millis();
  long endTime = currentMillis + 2 * 60 * 1000;
  while(millis()<endTime) {
    do {
      if( s%10 == 0 ){
        for(i=0; i<strip.numPixels(); i++) {
          if( random(1,80) < 2 ){
            strip.setPixelColor(i, WheelBlinky(s+100));
          } else if ( random(1,80) == 1 ){
            strip.setPixelColor(i, strip.Color(0,0,0));
          }
        }
        strip.show();
        delay(wait);
      }

      s = s + p;
    } while( s%80 != 0 );
    p = (p * -1) % 10;
    counter++;
  }

}


void rainbow(uint8_t wait) {
  uint16_t i, s, p;
  s = 0; 
  p = 0;
  int counter = 0;
  currentMillis = millis();
  long endTime = currentMillis + 2 * 60 * 1000;
  while(millis()<endTime) {
    do {
      if( s%10 == 0 ){
        for(i=0; i<strip.numPixels(); i++) {
          if( random(1,80) < 2 ){
            strip.setPixelColor(i, WheelBlinky(s+100));
          } else if ( random(1,80) == 1 ){
            strip.setPixelColor(i, strip.Color(0,0,0));
          }
        }
        strip.show();
        delay(wait);
      }

      s = s + p;
    } while( s%80 != 0 );
    p = p * -1;
    counter++;
  }
}

uint32_t WheelBlinky(byte WheelPos) {
  uint32_t wheel_offset = 2;
  uint32_t wheel3 = WheelPos * wheel_offset;
  uint32_t wheeldown = 255 - WheelPos * wheel_offset;
  uint32_t wheelnull = 0;

  if(WheelPos < 85) {
   return strip.Color(wheel3, wheeldown, wheelnull);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(wheeldown, wheelnull, wheel3);
  } else {
   WheelPos -= 170;
   return strip.Color(wheelnull, wheel3, wheeldown);
  }
}

///////////////
// playcloud //
///////////////

void playacloud(){
    for(int i = 0;i<PLAYA_LENGTH;i++){
      unsigned long startMillis= millis();  // Start of sample window
    float peakToPeak = 0;   // peak-to-peak level
  
    unsigned int signalMax = 0;
    unsigned int signalMin = 1023;
    unsigned int c, y;
  
  
    // collect data for length of sample window (in mS)
    while (millis() - startMillis < SAMPLE_WINDOW)
    {
      sample = analogRead(MIC_PIN);
      if (sample < 1024)  // toss out spurious readings
      {
        if (sample > signalMax)
        {
          signalMax = sample;  // save just the max levels
        }
        else if (sample < signalMin)
        {
          signalMin = sample;  // save just the min levels
        }
      }
    }
    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   
    // Serial.println(peakToPeak);
  
  
    //Fill the strip with rainbow gradient
    for (int i=0;i<=strip.numPixels()-1;i++){
      strip.setPixelColor(i,Wheel(map(i,0,strip.numPixels()-1,30,150)));
    }
  
  
    //Scale the input logarithmically instead of linearly
    c = fscale(INPUT_FLOOR, INPUT_CEILING, strip.numPixels(), 0, peakToPeak, 2);
  
    
  
  
    if(c < peak) {
      peak = c;        // Keep dot on top
      dotHangCount = 0;    // make the dot hang before falling
    }
    if (c <= strip.numPixels()) { // Fill partial column with off pixels
      drawLine(strip.numPixels(), strip.numPixels()-c, strip.Color(0, 0, 0));
    }
  
    // Set the peak dot to match the rainbow gradient
    y = strip.numPixels() - peak;
    
    strip.setPixelColor(y-1,Wheel(map(y,0,strip.numPixels()-1,30,150)));
    strip.setPixelColor(y-2,Wheel(map(y,0,strip.numPixels()-1,30,150)));
    strip.setPixelColor(y-3,Wheel(map(y,0,strip.numPixels()-1,30,150)));
    strip.setPixelColor(y-4,Wheel(map(y,0,strip.numPixels()-1,30,150)));
    strip.setPixelColor(y-5,Wheel(map(y,0,strip.numPixels()-1,30,150)));
    strip.setPixelColor(y-6,Wheel(map(y,0,strip.numPixels()-1,30,150)));
    strip.setPixelColor(y-7,Wheel(map(y,0,strip.numPixels()-1,30,150)));
  
    strip.show();
  
    // Frame based peak dot animation
    if(dotHangCount > PEAK_HANG) { //Peak pause length
      if(++dotCount >= PEAK_FALL) { //Fall rate 
        peak++;
        dotCount = 0;
      }
    } 
    else {
      dotHangCount++; 
    }
  }
}

//Used to draw a line between two points of a given color
void drawLine(uint8_t from, uint8_t to, uint32_t c) {
  uint8_t fromTemp;
  if (from > to) {
    fromTemp = from;
    from = to;
    to = fromTemp;
  }
  for(int i=from; i<=to; i++){
    strip.setPixelColor(i, c);
  }
}


float fscale( float originalMin, float originalMax, float newBegin, float
newEnd, float inputValue, float curve){

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;


  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output 
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  /*
   Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution  
   Serial.println(); 
   */

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin){ 
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd; 
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine 
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0){
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {   
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange); 
  }

  return rangedValue;
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 255) {
    return strip.Color(WheelPos * 255, 255 - WheelPos * 255, 255);
  } 
  else if(WheelPos < 255) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 255, 255, WheelPos * 255);
  } 
  else {
    WheelPos -= 255;
    return strip.Color(255, WheelPos * 255, 255 - WheelPos * 255);
  }
}


/////////////
// sparkle //
/////////////

void sparkle(uint32_t color) {
  int number_of_sparkles = 10;
  int sparkle_locations[number_of_sparkles];

  for(int i=0;i<SPARKLE_LENGTH;i++){
    for(int j=0;j<number_of_sparkles;j++){
      sparkle_locations[j] = random(N_PIXELS);
      strip.setPixelColor(sparkle_locations[j], color);
    }
    strip.show();
    delay(30);
    for(int j=0;j<number_of_sparkles;j++){
      strip.setPixelColor(sparkle_locations[j], 0);
    }
  }
    
}


