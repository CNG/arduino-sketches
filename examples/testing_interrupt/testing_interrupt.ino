/*
  Testing button predicability.

  buttonInterrupt() runs very fast and just sets "pushed" to true.
  The program loop runs checkButton() whenever convenient.

  Note the interrupt triggers at program start. (Why?)
 */

boolean pushed = false;

void setup() {
  pinMode(0, INPUT_PULLUP); // configure pin to behave as input
  attachInterrupt(2, buttonInterrupt, FALLING); // listen for high to low
  while (!Serial);
  Serial.begin(9600);
}

void buttonInterrupt() {
  pushed = true;
}

/**
 * See if button was pushed. If so, delay and then set "pushed" back to false.
 * If we don't delay first, the interrupt might set "pushed" back to true, 
 * causing the condition to fire too many times on "one" button push.
 */
void checkButton() {
  if( pushed ){
    // delay before false else interrupt might true too soon
    delay(250);
    pushed = false;
    Serial.println("");
    Serial.println("Pushed!");
  }
}

void loop() {

  Serial.print("x");
  delay(10);

  checkButton();

}
