#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_NeoPixel.h>
 
#define PIN 6
#define TPIXEL 128 //The total amount of pixel's/led's in your connected strip/stick (Default is 60)
 
int switchPin = 10; // switch is connected to pin 10
int val; // variable for reading the pin status
int val2;
int buttonState; // variable to hold the button state
int lightMode = 0; // how many times the button has been pressed
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TPIXEL, PIN, NEO_GRB + NEO_KHZ800);
// our RGB -> eye-recognized gamma color
byte gammatable[256];
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
 
void setup() {
Serial.begin(9600); // Set up serial communication at 9600bps
pinMode(switchPin, INPUT_PULLUP); // Set the switch pin as input
pinMode(PIN, OUTPUT);
strip.setBrightness(160); //adjust brightness here
buttonState = digitalRead(switchPin); // read the initial state
strip.begin();
strip.show(); // Initialize all pixels to 'off'
if (tcs.begin()) {
Serial.println("Found sensor");
} else {
Serial.println("No TCS34725 found ... check your connections");
while (1); // halt!
}
// thanks PhilB for this gamma table!
// it helps convert RGB colors to what humans see
for (int i=0; i<256; i++) {
float x = i;
x /= 255;
x = pow(x, 2.5);
x *= 255;
gammatable[i] = x;
//Serial.println(gammatable[i]);
}
for (int i=0; i<3; i++){ //this sequence flashes the first pixel three times as a countdown to the color reading.
strip.setPixelColor (0, strip.Color(188, 188, 188)); //white, but dimmer-- 255 for all three values makes it blinding!
strip.show();
delay(1000);
strip.setPixelColor (0, strip.Color(0, 0, 0));
strip.show();
delay(500);
}
uint16_t clear, red, green, blue;
 
tcs.setInterrupt(false); // turn on LED
 
delay(60); // takes 50ms to read
tcs.getRawData(&red, &green, &blue, &clear);
tcs.setInterrupt(true); // turn off LED
Serial.print("C:\t"); Serial.print(clear);
Serial.print("\tR:\t"); Serial.print(red);
Serial.print("\tG:\t"); Serial.print(green);
Serial.print("\tB:\t"); Serial.print(blue);
// Figure out some basic hex code for visualization
uint32_t sum = red;
sum += green;
sum += blue;
sum = clear;
float r, g, b;
r = red; r /= sum;
g = green; g /= sum;
b = blue; b /= sum;
r *= 256; g *= 256; b *= 256;
Serial.print("\t");
Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX);
Serial.println();
 
Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" "); Serial.println((int)b );
 colorWipe(strip.Color(gammatable[(int)r], gammatable[(int)g], gammatable[(int)b]), 0);
}
 
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(20);
  }
}
 
void loop(){
  //rain();
  val = digitalRead(switchPin); // read input value and store it in val
  delay (20);
  val2 = digitalRead(switchPin);
  if (val == val2) {
    if (val != buttonState && val==LOW) { // the button state has changed!
        if (lightMode == 0) {
          lightMode = 1;
        }
      else if (lightMode == 1) {
          lightMode = 2;
        }
      else if (lightMode == 2){
          lightMode = 3;
          //delay (20);
        }
      else if (lightMode == 3) {
          lightMode = 0;
        }
      }
    }

buttonState = val; // save the new state in our variable
if (lightMode == 0) {
strip.show();
}
if (lightMode == 1) {
rainbow(10);
delay(20);
}
if (lightMode == 2) {
rain();
delay(20);
}
if (lightMode == 3) {
rainbowCycle(10);
delay(20);
  }
}
// Rain Program
 
void rain() {
// Create an array of 20 raindrops
  const int count = 20;
  int pos[count];
// Set each rain drop at the starting gate.
// Signify by a position of -1
for( int i=0; i < count; i++) {
pos[i]=-1;
}
// Main loop. Keep looping until we've done
// enough "frames."
boolean done=false;
int counter = 0;
while(!done) {
// Start by turning all LEDs off:
for(int i=0; i<strip.numPixels(); i++)
strip.setPixelColor(i, 0);
 
// Loop for each rain drop
for( int i=0; i < count; i++) {
// If the drop is out of the starting gate,
// turn on the LED for it.
if( pos[i] >= 0 ) {
strip.setPixelColor(pos[i], strip.Color(0,0,127));
// Move the drop down one row
pos[i] -= 7;
// If we've fallen off the strip, but us back at the starting gate.
if( pos[i] < 0 )
pos[i]=-1;
}
// If this drop is at the starting gate, randomly
// see if we should start it falling.
if ( pos[i] == -1 && random(40) == 0 && counter < 380 ) {
// Pick one of the 6 starting spots to begin falling
pos[i] = 143-random(6);
}
strip.show();
delay(2);
}
 
}
}
//Rainbow Program
void rainbow(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
strip.setPixelColor(i, Wheel((i+j) & 255));
}
strip.show();
    delay(wait);
  }
}

// Rainbow Cycle Program - Equally distributed
void rainbowCycle(uint8_t wait) {
uint16_t i, j;
 
for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
for(i=0; i< strip.numPixels(); i++) {
strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
}
strip.show();
delay(wait);
}
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
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
