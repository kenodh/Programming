const int buzzer = 11; //buzzer to arduino pin 9

void setup(){
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
}

void loop(){
  tone(buzzer, 1000); // Send 1KHz sound signal...
  // digitalWrite(buzzer, HIGH);
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  // digitalWrite(buzzer, LOW);
  delay(1000);        // ...for 1sec
}