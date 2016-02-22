/*
  This program takes FHT_N readings from an attached microphone and prints
  averages of AVE_N values on each line. Then it prints FHT_N / AVE_N values
  read separately with a delay of AVE_N microseconds between each reading.

  It seems the arbitrary delay has a similar effect of more readings averaged in
  consideration of the cleanliness of resulting graph.

  Tested on Arduino DUE (https://www.adafruit.com/products/1076)
  
  Method of getting values from free running ADC from forum user "stimmer":
  http://forum.arduino.cc/index.php?topic=137635.5
  and explained on blog post at
  http://frenki.net/2013/10/fast-analogread-with-arduino-due/

*/

#define FHT_N   10000 // typical FHT function needs 256 readings
#define AVE_N   10

unsigned long groups = floor( FHT_N / AVE_N );

unsigned long fht_input[FHT_N];
unsigned long fht_input_delay[FHT_N / AVE_N];
unsigned long average;
float value, volts;


void setup() {
  while (!Serial); // wait for serial monitor to be opened
  Serial.begin(9600);
  ADC->ADC_MR |= 0x80;  //set free running mode on ADC
  ADC->ADC_CHER = 0x80; //enable ADC on pin A0
}


void loop() {
  unsigned int i, j;

  delay(2*1000);

  for (i=0; i<FHT_N; i++) {
    while((ADC->ADC_ISR & 0x80)==0); // wait for conversion
    fht_input[i]=ADC->ADC_CDR[7]; //get values
  }

  for (i=0; i<groups; i++) {
    while((ADC->ADC_ISR & 0x80)==0); // wait for conversion
    fht_input_delay[i]=ADC->ADC_CDR[7]; //get values
    delayMicroseconds(AVE_N);
  }

  Serial.println("=== AVERAGED READINGS ===");

  for (i=0; i<groups; i++) {
    average = 0;
    for (j=0; j<AVE_N; j++) {
      average += fht_input[ i * AVE_N + j ];
    }
    value = (float)average / (float)AVE_N;
    volts = value * 3.3 / 4095.0;
    Serial.println(volts, 6);
  }

  Serial.println("=== DELAYED READINGS ===");

  for (i=0; i<groups; i++) {
    value = (float)fht_input_delay[i];
    volts = value * 3.3 / 4095.0;
    Serial.println(volts, 6);
  }

  delay(20000*1000);

}

