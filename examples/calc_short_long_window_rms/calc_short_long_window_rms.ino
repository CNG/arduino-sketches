/*
  This program takes FHT_N readings from an attached microphone and then prints 
  them separated by commas on a single line. The output can be copied into a 
  text file and saved as a .CSV file for subsequent opening in a spreadsheet 
  application. 

  Tested on Adafruit FLORA (https://www.adafruit.com/products/659)
*/

#define MIC_PIN A10  // Microphone is attached to this analog pin
#define SHORTWINDOW 216
#define LONGWINDOW 22 // LONGWINDOW * SHORTWINDOW = samples in LONGWINDOW

int signalbuffer[SHORTWINDOW];
long rmsbuffer[LONGWINDOW];
byte cb_next = 0;

void setup() {
  while (!Serial); // wait for serial monitor to be opened
  Serial.begin(9600);

  // init rmsbuffer to 0
  for (int i=0; i<LONGWINDOW; i++) {
    rmsbuffer[i] = 0;
  }
}


void loop() {
  int i;
  while(1) { // reduces jitter
    cli(); // disable interrupts
    for (i=0; i<SHORTWINDOW; i++) {
      signalbuffer[i] = analogRead(MIC_PIN);
    }
    sei(); // reenable interrupts

    // calculate RMS
    long rms = 0;
    for(i=0; i<SHORTWINDOW; i++){      
      signalbuffer[i] = signalbuffer[i] - 512;
      long sample = signalbuffer[i];
      long squared = sample * sample;
      rms = rms + squared;
    }
    rms = sqrt(rms);

    // Push to rms circular buffer
    rmsbuffer[cb_next] = rms;
    cb_next = (cb_next + 1) % LONGWINDOW; // increment circular buffer index

    // Calculate 1 second rms
    long rms_1sec = 0;
    for(i=0; i<LONGWINDOW; i++) {      
      rms_1sec += rmsbuffer[i];
    }
    rms_1sec = rms_1sec / LONGWINDOW;
    
    Serial.print(rms); Serial.print(","); Serial.print(rms_1sec); Serial.println();
//
//    for(i=0; i<SHORTWINDOW; i++){      
//      Serial.print(signalbuffer[i]);
//      Serial.print(",");
//    }
  
  }
}

