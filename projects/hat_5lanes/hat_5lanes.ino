#include "utilities.h"
#include "Lights.h"
#include "VolumeMeter.h"

//#define DEBUG
#ifdef DEBUG
 #define PRINT(x) Serial.print (x)
 #define PRINTLN(x) Serial.println (x)
#else
 #define PRINT(x)
 #define PRINTLN(x)
#endif


const byte
  pin_microphone = A0,  // Microphone is attached to this analog pin
  pin_lights = 6,  // NeoPixel LED strand is connected to this pin
  pin_brightness = A1,  // using 1K potentiometer
  pin_secondary = A2,  // other setting, like color or speed, using 1K pot
  pin_mode_button = SCL,
  pin_unused = A3;  // For seeding random number generator
const word
  pixels_tall = 5,
  pixels_wide = 120,
  peak_fall_rate = 5,
  num_samples = 10;

Lights lights (pin_lights, pixels_wide, pixels_tall);
VolumeMeter volume_meter (
  pin_microphone, lights.mirrored_width, peak_fall_rate, num_samples
  );

volatile boolean button_pushed;
byte mode;  // Current program mode; button push increments
byte brightness;  // Store brightness from pot for passing to functions
byte level;  // Store value from secondary pot for other settings


void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
    while( !Serial );
  #endif

  // Set the button pin as input and listen for high to low
  pinMode(pin_mode_button, INPUT_PULLUP);
  attachInterrupt(pin_mode_button, buttonInterrupt, FALLING);

  randomSeed(analogRead(pin_unused));
  lights.setBrightnessPin(pin_brightness);
  mode = random(2,5); // how many times the button has been pressed
  mode = 4;
  PRINTLN("Done setting up!");
}



void loop() {
  checkButton();
  brightness = lights.setBrightness();
  lights.show();
  switch (mode) {
    default: // rollover
      mode = 0; break;
    case 1: lights.rainbow(lights.mirrored_width, 0, level, 32, true, false); break;
    case 2: lights.rainbow(lights.width, 0, level, 256, false, true); break;
    case 3: modeVolMeter(); break;
    case 4: modeAnts(); break;
    //case 4: modePulseSlow(); break;
    //case 5: modePulseFast(); break;
    
    //brokish: case : modePulseSlow(); break;
    //brokish: case : modePulseFast(); break;
  }
}


// Toggle variable and check later. Doing work directly within
// interrupt can cause weirdness due to variable volatility.
void buttonInterrupt() {
  button_pushed = true;
}


// See if button was pushed. If so, delay and then set button_pushed back to false.
// If we don't delay first, the interrupt might set button_pushed back to true, 
// causing the condition to fire too many times on one button push.
boolean checkButton() {
  if ( button_pushed ) {
    PRINTLN("button_pushed");
    lights.setBrightness();
    level = analogRead(pin_secondary) / 4;
    delay(250);  // Maybe better to set a timer?
    button_pushed = false;
    mode++;
    return true;
  }
  return button_pushed;
}


void modeVolMeter() {
  static byte
    startColor = 60,
    endColor   = 250;
  static Levels levels;
  static word i;
  static word max_height = volume_meter.max_height;

  while(true) {
    if (checkButton()) { return; };
    levels = volume_meter.sample();
    PRINT(levels.height); PRINT(" "); PRINTLN(levels.peak);

    // Color pixels based on rainbow gradient.
    for (i=0; i<max_height-2; i++) {
      if (i >= levels.height) { 
        lights.setMirroredPixelColor(
          max_height-i-1,
          0, 0, 0
          );
      } else {
        lights.setMirroredPixelColor(
          max_height-i,
          Wheel(map(i, 0, max_height-1, startColor, endColor))
          );
      }
    }

    // Draw peak dot.
    if (levels.peak > 0 && levels.peak <= max_height-1) {
      lights.setMirroredPixelColor(
        max_height-levels.peak,
        Wheel(map(
          max_height-levels.peak,
          0, max_height-1,
          startColor, endColor
          ))
        );
    }

    lights.show();
  }
}


void modeAnts() {
  byte times[]  = { 0, 1, 1, 2, 2, 3, 4 };
  byte spaces[] = { 7, 5, 5, 4, 3, 3, 0 };
  for (byte i = 0; i < sizeof(times); i++) {
    if (checkButton()) { return; };
    lights.raincrazy( 5, times[i], spaces[i], i%2==0, random(0,8) == 0 && spaces[i] > 0, random(0,2000) == 0 );
  }
}


void modePulseSlow() {
    if (checkButton()) { return; };
    lights.colorWipe(Color(0, 0, 0), pixels_wide-1, 0, 0);
    delay(1000);
    if (checkButton()) { return; };
    lights.pulse2(Color(255,0,0), brightness);
    delay(5000);
    // lights.pulse2(Color(128,128,0), brightness);
    // delay(5000);
    if (checkButton()) { return; };
    lights.pulse2(Color(0,255,0), brightness);
    delay(5000);
    // lights.pulse2(Color(0,128,128), brightness);
    // delay(5000);
    if (checkButton()) { return; };
    lights.pulse2(Color(0,0,255), brightness);
    // delay(5000);
    // lights.pulse2(Color(128,0,128), brightness);
    // delay(5000);
    // lights.pulse2(Color(125,125,125), brightness);
    // delay(20);
}


void modePulseFast() {
    if (checkButton()) { return; };
    lights.pulse(Color(255,0,0), brightness);
    if (checkButton()) { return; };
    lights.pulse(Color(128,128,0), brightness);
    if (checkButton()) { return; };
    lights.pulse(Color(0,255,0), brightness);
    if (checkButton()) { return; };
    lights.pulse(Color(0,128,128), brightness);
    if (checkButton()) { return; };
    lights.pulse(Color(0,0,255), brightness);
    if (checkButton()) { return; };
    lights.pulse(Color(128,0,128), brightness);
    if (checkButton()) { return; };
    lights.pulse(Color(125,125,125), brightness);
    // whiteFlash(60);
}
