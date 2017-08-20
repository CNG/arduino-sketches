// 2930 reads per second on Adafruit Metro Mini
// (453*256)/(39654-80)*1000

int counter = 0;

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
  if(counter==255){
    Serial.println("");
    Serial.print(millis());
    Serial.print(" ");
    counter = 0;
  }
  Serial.print(ADCH);
  Serial.print(" ");
  counter++;
}

void loop(){ }
