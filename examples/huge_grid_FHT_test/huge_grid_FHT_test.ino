#define OCT_NORM 0
#define OCTAVE 1 // use the octave output function
#define FHT_N 256 // set to 256 point fht

#define MIC_PIN   A0  // Microphone is attached to this analog pin
#define ROWS       8  // NeoPixel LED strand is connected to this pin
#define COLS      32  // NeoPixel LED strand is connected to this pin
#define LED_PIN   22  // NeoPixel LED strand is connected to this pin
#define SAMPLES   10  // Length of buffer for dynamic level adjustment
#define BRIGHTNESS 128

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <FHT.h> // include the library

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(COLS, ROWS, LED_PIN,
  NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

byte volCount = 0;
int noise[]={136,120,31,38,45,51,56,53}; // manual adjust ave
//int noise[]={129,114,31,41,47,52,57,56}; // auto adjust max
//int noise[]={129,114,27,37,43,50,55,52}; // auto adjust ave
//int noise[]={0,0,0,0,0,0,0,0};

int
  vol[6][SAMPLES],
  fht_noise_adjusted[8],
  ColourSpectrum[6],
  minLvlAvg[6],
  maxLvlAvg[6],
  minLvl[6],
  maxLvl[6],
  height[6];

void setup() {
  delay(200);
  //while (!Serial);
  for( uint8_t i = 0; i<6; i++ ){
    minLvlAvg[i] = 0;      // For dynamic adjustment of graph low & high
    maxLvlAvg[i] = 16;
  }
  Serial.begin(9600);
  matrix.begin();
  matrix.setBrightness(BRIGHTNESS);
  matrix.fillScreen(matrix.Color(0, 0, 0));
  matrix.show();
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

void loop() {

  //while(1){Serial.println(analogRead(MIC_PIN)); delay(1);}

  matrix.fillScreen(matrix.Color(0, 0, 0));

  cli();  // UDRE interrupt slows this way down on arduino1.0
  for (int i = 0 ; i < FHT_N ; i++) { // save samples
    fht_input[i] = analogRead(MIC_PIN); // put real data into bins
  }
  fht_window(); // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run(); // process the data in the fht
  fht_mag_octave(); // take the output of the fht
  sei(); // re-enable interrupts

  for (int i = 0; i < 6; i++) {  // For each of the 6 useful octave bins
    fht_noise_adjusted[i] = abs(fht_oct_out[i+2]-noise[i+2]);  // take the pink noise average level out, take the asbolute value to avoid negative numbers
    //fht_noise_adjusted[i] = constrain(fht_noise_adjusted[i], 10, 125); // 37 lowpass for noise; 125 high pass doesn't go much higher than this [found by trial and error]

    // Calculate bar height based on dynamic min/max levels (fixed point):
    height[i] = ROWS * ( fht_noise_adjusted[i] - minLvlAvg[i] ) / (long)( maxLvlAvg[i] - minLvlAvg[i] );
    if(height[i] < 0L) height[i] = 0; // Clip output
    else if(height[i] > ROWS) height[i] = ROWS;

    vol[i][volCount] = fht_noise_adjusted[i]; // Save sample for dynamic leveling
 
    // Get volume range of prior frames
    minLvl[i] = maxLvl[i] = vol[i][0];
    for(uint8_t j=1; j<SAMPLES; j++) {
      if(vol[i][j] < minLvl[i])      minLvl[i] = vol[i][j];
      else if(vol[i][j] > maxLvl[i]) maxLvl[i] = vol[i][j];
    }
    if((maxLvl[i] - minLvl[i]) < ROWS) maxLvl[i] = minLvl[i] + ROWS;
    minLvlAvg[i] = (minLvlAvg[i] * 63 + minLvl[i]) >> 6; // Dampen min/max levels
    maxLvlAvg[i] = (maxLvlAvg[i] * 63 + maxLvl[i]) >> 6; // (fake rolling average)

    for(int y = 0; y < ROWS ; y++){
      if( y < height[i] ){
        matrix.drawPixel(5*i+1,y,matrix.Color(200, 0, 255));
        matrix.drawPixel(5*i+2,y,matrix.Color(200, 0, 255));
        matrix.drawPixel(5*i+3,y,matrix.Color(200, 0, 255));
        matrix.drawPixel(5*i+4,y,matrix.Color(200, 0, 255));
        matrix.drawPixel(5*i+5,y,matrix.Color(200, 0, 255));
      }
    }

  }
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter

  matrix.show();

}





