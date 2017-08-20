// 2877 reads per second on Adafruit Metro Mini
// 1574*64/(35028-18)*1000

byte pending = 0;
int vals[63];
int prev_vals[63];
int counter = 0;
int i = 0;
char buffer[255];

void setup(){
  Serial.begin(115200);
  cli(); // disable interrupts
  ADCSRA = 0;
  ADCSRB = 0;
  ADMUX |= (1 << REFS0); // set reference voltage
  ADMUX |= (1 << ADLAR); // left align the ADC value so we can read highest 8 bits from ADCH register only
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); // set ADC clock with 32 prescaler- 16mHz/32=500kHz
  ADCSRA |= (1 << ADATE); // enabble auto trigger
  ADCSRA |= (1 << ADIE); // enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN); // enable ADC
  ADCSRA |= (1 << ADSC); // start ADC measurements
  sei(); // enable interrupts
}

ISR(ADC_vect) {
  vals[counter] = ADCH; //get value from A0
  if(counter==63){
    memcpy( prev_vals, vals, 63*sizeof(int) );
    pending = 1;
    counter = 0;
  } else {
    counter++;
  }
}

void loop(){

  if(pending==1){
    Serial.print(millis());
    Serial.print(" ");
    for(i=0;i<4;i++){
      sprintf (buffer, "%u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u ",
        prev_vals[i*16],
        prev_vals[i*16+1],
        prev_vals[i*16+2],
        prev_vals[i*16+3],
        prev_vals[i*16+4],
        prev_vals[i*16+5],
        prev_vals[i*16+6],
        prev_vals[i*16+7],
        prev_vals[i*16+8],
        prev_vals[i*16+9],
        prev_vals[i*16+10],
        prev_vals[i*16+11],
        prev_vals[i*16+12],
        prev_vals[i*16+13],
        prev_vals[i*16+14],
        prev_vals[i*16+15]
      );
      Serial.print(buffer);
    }
    Serial.println();
  }

}
