
#include <Adafruit_NeoPixel.h>

#define N_PIXELS  25  // Number of pixels in strand
#define MIC_PIN   12  // Microphone is attached to this analog pin
#define LED_PIN    6  // NeoPixel LED strand is connected to this pin
#define DC_OFFSET  0  // DC offset in mic signal - if unusure, leave 0
#define SAMPLES   50  // Length of buffer for dynamic level adjustment
#define SAMPLESFHT 10  // Length of buffer for dynamic level adjustment
#define TOP       28  // Allow dot to go slightly off scale
#define PEAK_FALL 20  // Rate of peak falling dot

#define LOG_OUT 1 // use the log output function
#define FHT_N 16 // set to 256 point fht

#include <FHT.h> // include the library

byte
  peak      = 0,      // Used for falling dot
  dotCount  = 0,      // Frame counter for delaying dot-falling speed
  volCount  = 0;      // Frame counter for storing past volume data
int
  vol[SAMPLES],       // Collection of prior volume samples
  volFHT[SAMPLESFHT],       // Collection of prior volume samples
  lvl       = 10,     // Current "dampened" audio level
  minLvlAvg = 0,      // For dynamic adjustment of graph low & high
  maxLvlAvg = 512;
Adafruit_NeoPixel
  strip = Adafruit_NeoPixel(N_PIXELS*4, LED_PIN, NEO_GRB + NEO_KHZ800);
int noise = 1;
boolean pushed = false;

int switchPin = 0; // switch is connected to pin 0
int lightMode = 3; // how many times the button has been pressed

void setup() {
  pinMode(switchPin, INPUT_PULLUP); // Set the switch pin as input
  attachInterrupt(2, buttonPushed, FALLING); // listen for high to low

  memset(vol, 0, sizeof(vol));
  //Serial.begin(9600);
  noise = ( analogRead(MIC_PIN) % 9 ) * 20;
  strip.begin();
}

// Instead of changing brightness directly within interrupt, we'll just toggle a
// variable that we'll check for later. This prevents any weirdness due to 
// variable volatility.
void buttonPushed() {
  pushed = true;
  lightMode++;
  if (lightMode == 5) lightMode = 1;
}

void setMeterPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
  strip.setPixelColor(n,r,g,b);
  strip.setPixelColor(49-n,r,g,b);
  strip.setPixelColor(50+n,r,g,b);
  strip.setPixelColor(99-n,r,g,b);
}
void setMeterPixelColor(uint16_t n, uint32_t c){
  strip.setPixelColor(n,c);
  strip.setPixelColor(49-n,c);
  strip.setPixelColor(50+n,c);
  strip.setPixelColor(99-n,c);
}
void setNormalPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
  strip.setPixelColor(n,r,g,b);
  strip.setPixelColor(99-n,r,g,b);
}
void setNormalPixelColor(uint16_t n, uint32_t c){
  strip.setPixelColor(n,c);
  strip.setPixelColor(99-n,c);
}

void mode0(){
    if(pushed){ pushed = false; return; }
    colorWipe(strip.Color(0, 0, 0), 0);

    delay(1000);
    if(pushed){ pushed = false; return; }
    pulse2(strip.Color(255,0,0));
    delay(5000);
//    pulse2(strip.Color(128,128,0));
//    delay(5000);
    if(pushed){ pushed = false; return; }
    pulse2(strip.Color(0,255,0));
    delay(5000);
//    pulse2(strip.Color(0,128,128));
//    delay(5000);
    if(pushed){ pushed = false; return; }
    pulse2(strip.Color(0,0,255));
//    delay(5000);
//    pulse2(strip.Color(128,0,128));
//    delay(5000);
//    pulse2(strip.Color(125,125,125));

    //delay(20);
}
void mode1(){
    
    if(pushed){ pushed = false; return; }
    pulse(strip.Color(255,0,0));
    if(pushed){ pushed = false; return; }
    pulse(strip.Color(128,128,0));
    if(pushed){ pushed = false; return; }
    pulse(strip.Color(0,255,0));
    if(pushed){ pushed = false; return; }
    pulse(strip.Color(0,128,128));
    if(pushed){ pushed = false; return; }
    pulse(strip.Color(0,0,255));
    if(pushed){ pushed = false; return; }
    pulse(strip.Color(128,0,128));
    if(pushed){ pushed = false; return; }
    pulse(strip.Color(125,125,125));
    //whiteFlash(60);
}
void mode2(){
    //rainbow(1);
    //rainbowCycle(1);
    for(uint16_t j=0; j<32; j++) {
      if(pushed){ pushed = false; return; }
      for(uint16_t i=0; i<50; i++) {
        setNormalPixelColor(i, Wheel((i+(j*8-1)) & 255));
      }
      strip.show();
      delay(50);
    }
}
void mode3(){
  while(true){
    if(pushed){ pushed = false; return; }
    volMeter(lightMode);
  }
}
void mode4(){
    if(pushed){ pushed = false; return; }
    volMeterFHT(lightMode);
}
void mode5(){
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
        if(pushed){ pushed = false; return; }
        rainbow2( 5, wait, space, j%2==0, random(0,8) == 0 && space > 0, random(0,20) == 0 );
      }
    }
}

void loop() {

  if (lightMode == 0) {
    mode0();
  } else if (lightMode == 1) {
    mode1();
  } else if (lightMode == 2) {
    mode2();
  } else if (lightMode == 3) {
    mode3();
  } else if (lightMode == 4) {
    mode4();
  } else {
    mode5();
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

  for(uint16_t i=0; i<N_PIXELS*2; i++) {
      setNormalPixelColor(i, strip.Color(255, 255, 255));
  }
  strip.show();
  delay(w);
  for(uint16_t i=0; i<N_PIXELS*2; i++) {
      setNormalPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  delay(w);


}

void pad( int number, byte width = 3 ) {
 int currentMax = 10;
 for (byte i=1; i<width; i++){
   if (number < currentMax) {
     Serial.print("0");
   }
   currentMax *= 10;
 }
 Serial.print(number);
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

    for(pixel=0; pixel<N_PIXELS*2; pixel++) {
      if(mode==1||mode==2){
        // every pixel different color
        setNormalPixelColor(pixel, Wheel((pixel+color) & 255));
      }else if(mode==3||mode==4){
        // every pixel same color
        setNormalPixelColor(pixel, Wheel(color));
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
                  setNormalPixelColor(pixel, Wheel(approx));
                } else {
                  setNormalPixelColor(pixel, Wheel(color));
                }
              } else {
                setNormalPixelColor(pixel, strip.Color(0, 0, 0));
              }
            } else {
              if( color % chunkSize == ( spaces - space) ) {
                if( solid ){
                  uint16_t approx, colorDist;
                  colorDist = 255 / 6;
                  approx = color - (color % colorDist);
                  setNormalPixelColor(pixel, Wheel(approx));
                } else {
                  setNormalPixelColor(pixel, Wheel(color));
                }
              } else {
                setNormalPixelColor(pixel, strip.Color(0, 0, 0));
              }
            }
          }
        }
  
      }
    }
    strip.show();
    delay(wait);
    if(mode==2||mode==4||mode==6){
      for(uint8_t i=0; i<N_PIXELS*2; i++) {
        setNormalPixelColor(i, strip.Color(0, 0, 0));
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

void volMeterFHT(int lightMode){
      //delay(50);
  lightMode = lightMode - 1;
  uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      n, height;
  int bucket = 0;

  int startColor, endColor;
  startColor = 60;
  endColor   = 250;

  cli();  // UDRE interrupt slows this way down on arduino1.0
  for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
    int n;
    n   = analogRead(MIC_PIN);
    n   = abs(n - 512); // Center on zero
    n   = (n <= noise) ? 0 : (n - noise); // Remove noise/hum
    //lvl = ((lvl * 7) + n) >> 3; // "Dampened" reading (else looks twitchy)
    fht_input[i] = n; // put real data into bins
  }
  fht_window(); // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run(); // process the data in the fht
  fht_mag_log(); // take the output of the fht
  sei();

  lvl = ((lvl * 7) + fht_log_out[bucket]) >> 3; // "Dampened" reading (else looks twitchy)

  //for (int i = 0; i < FHT_N / 2 - 1 ; i++) {
  //  pad(lvl);
  //  Serial.print(' '); // send out the data
 // }
  //pad(fht_log_out[FHT_N/2-1]);
  //Serial.println(' ');

  //Serial.println(noise);

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

  if(height < 0L)       height = 0;      // Clip output
  else if(height > TOP) height = TOP;
  if(height > peak)     peak   = height; // Keep 'peak' dot at top

  // Color pixels based on rainbow gradient
  setMeterPixelColor(N_PIXELS-1,Wheel(map(0,0,N_PIXELS-1,startColor,endColor)));
  setMeterPixelColor(N_PIXELS-2,Wheel(map(1,0,N_PIXELS-1,startColor,endColor)));
  for(i=0; i<N_PIXELS-2; i++) {
    if(i >= height){ 
      setMeterPixelColor(N_PIXELS-i-1,   0,   0, 0);
    } else {
      setMeterPixelColor(N_PIXELS-i,Wheel(map(i,0,N_PIXELS-1,startColor,endColor)));
    }
  }

  // Draw peak dot  
  if(peak > 0 && peak <= N_PIXELS-1){
    setMeterPixelColor(N_PIXELS-peak,Wheel( map(N_PIXELS-peak,0,N_PIXELS-1,startColor,endColor) ));
  }
  
  strip.show(); // Update strip

  // Every few frames, make the peak pixel drop by 1:
  if(++dotCount >= PEAK_FALL) { 
    if(peak > 0) peak--;
    dotCount = 0;
  }

  volFHT[volCount] = lvl;                      // Save sample for dynamic leveling
  if(++volCount >= SAMPLESFHT) volCount = 0; // Advance/rollover sample counter

  // Get volume range of prior frames
  minLvl = maxLvl = volFHT[0];
  for(i=1; i<SAMPLESFHT; i++) {
    if(volFHT[i] < minLvl)      minLvl = volFHT[i];
    else if(volFHT[i] > maxLvl) maxLvl = volFHT[i];
  }
  if((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)

}

void volMeter(int lightMode){
  lightMode = lightMode - 1;
  uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      n, height;

  int startColor, endColor;
  startColor = 60;
  endColor   = 250;

  n   = analogRead(MIC_PIN); // Raw reading from mic 
  n   = abs(n - 512 - DC_OFFSET); // Center on zero
  n   = (n <= noise) ? 0 : (n - noise); // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3; // "Dampened" reading (else looks twitchy)

  //Serial.println(noise);

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

  if(height < 0L)       height = 0;      // Clip output
  else if(height > TOP) height = TOP;
  if(height > peak)     peak   = height; // Keep 'peak' dot at top

  // Color pixels based on rainbow gradient
  setMeterPixelColor(N_PIXELS-1,Wheel(map(0,0,N_PIXELS-1,startColor,endColor)));
  setMeterPixelColor(N_PIXELS-2,Wheel(map(1,0,N_PIXELS-1,startColor,endColor)));
  for(i=0; i<N_PIXELS-2; i++) {
    if(i >= height){ 
      setMeterPixelColor(N_PIXELS-i-1,   0,   0, 0);
    } else {
      setMeterPixelColor(N_PIXELS-i,Wheel(map(i,0,N_PIXELS-1,startColor,endColor)));
    }
  }

  // Draw peak dot  
  if(peak > 0 && peak <= N_PIXELS-1){
    setMeterPixelColor(N_PIXELS-peak,Wheel( map(N_PIXELS-peak,0,N_PIXELS-1,startColor,endColor) ));
  }
  
  strip.show(); // Update strip

  // Every few frames, make the peak pixel drop by 1:
  if(++dotCount >= PEAK_FALL) { 
    if(peak > 0) peak--;
    dotCount = 0;
  }

  vol[volCount] = n;                      // Save sample for dynamic leveling
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter

  // Get volume range of prior frames
  minLvl = maxLvl = vol[0];
  for(i=1; i<SAMPLES; i++) {
    if(vol[i] < minLvl)      minLvl = vol[i];
    else if(vol[i] > maxLvl) maxLvl = vol[i];
  }
  if((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)

}

//Rainbow Program
void rainbow(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<255; j++) {
    for(i=0; i<50; i++) {
      setNormalPixelColor(i, Wheel((i+j) & 255));
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
