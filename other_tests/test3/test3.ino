int PIN_LED = 6;
int PINS_RGB[3] = {9, 10, 11};

void setup() {
  pinMode(7, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  
  analogWrite(PIN_LED, 0);
  
  delay(1000);
  
  analogWrite(PIN_LED, 255);
}
