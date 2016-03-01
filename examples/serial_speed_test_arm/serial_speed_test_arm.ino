void setup() {
  while (!Serial);
  Serial.begin(115200);
  SerialUSB.begin(1); // baud rate ignored? http://forum.arduino.cc/index.php?topic=132811.0
}

unsigned long start, end;

void loop() {

  start = millis();
  for(int i = 0; i < 500; i++){
    Serial.print(i);
    if( i == 500 - 1 ){
      Serial.println();
    } else {
      Serial.print(",");
    }
  }
  end = millis();
  Serial.print("Serial.print milliseconds: "); Serial.println(end - start);
  // Serial.begin(9600)   -> Serial.print 500 loop milliseconds: 1964
  // Serial.begin(115200) -> Serial.print 500 loop milliseconds: 159


  start = millis();
  for(int i = 0; i < 500; i++){
    SerialUSB.print(i);
    if( i == 500 - 1 ){
      SerialUSB.println();
    } else {
      SerialUSB.print(",");
    }
  }
  end = millis();
  SerialUSB.print("SerialUSB.print milliseconds: "); SerialUSB.println(end - start);
  // SerialUSB.print 500 loop milliseconds: 16

}