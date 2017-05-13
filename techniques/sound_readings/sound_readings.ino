/*
  This program takes FHT_N readings from an attached microphone and then prints 
  them separated by commas on a single line. The output can be copied into a 
  text file and saved as a .CSV file for subsequent opening in a spreadsheet 
  application. 

  Tested on Adafruit FLORA (https://www.adafruit.com/products/659)
*/


#define MIC_PIN A10  // Microphone is attached to this analog pin
#define FHT_N   256 // typical FHT function needs 256 readings
int fht_input[FHT_N], i;


void setup() {
  while (!Serial); // wait for serial monitor to be opened
  Serial.begin(9600);
}


void loop() {
  
  Serial.print(millis());
  Serial.print(",");

  for (i=0; i<FHT_N; i++) {
    fht_input[i] = analogRead(MIC_PIN);
  }
  
  for (i=0; i<FHT_N; i++) {
    Serial.print(fht_input[i]);Serial.print(",");
  }

  Serial.println();
      
}
