
#include <SPI.h>
#include <RFID.h>
#include <Wire.h> 
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#define SS_PIN 10
#define RST_PIN 9

Servo servo;
int angle = 10;
const int buzzer = 11;
LiquidCrystal_I2C lcd(0x27,16,2);
RFID rfid(SS_PIN, RST_PIN);
String rfidCard;
void buzz(){
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer); 
  delay(1000);
}
void buzz2(){
  for(int i=0;i<5;i++){
  tone(buzzer, 1300); // Send 1KHz sound signal...
  delay(500);        // ...for 1 sec
  noTone(buzzer); 
  delay(500);
  }
}
void setup() {
  lcd.init(); 
  lcd.backlight();
  pinMode(buzzer, OUTPUT);
  servo.attach(9);
  servo.write(angle);
  Serial.begin(9600);
  Serial.println("Starting the RFID Reader...");
  SPI.begin();
  rfid.init();
}
void opendoor(){
  for(angle = 10; angle < 180; angle++)  
  {         
    Serial.println("in od");                         
    servo.write(angle);               
    delay(15);                   
  } 
  delay(3000);
  // now scan back from 180 to 0 degrees
  for(angle = 180; angle > 10; angle--)    
  {                                
    servo.write(angle);           
    delay(15);       
  } 
}
void loop() {
  char *oldcards[3] = {"70 16 114 161", "99 236 185 1", "105 223 37 59"}; // tag, card-original-yellow, card2
  char *blacklist[1] = {"105 255 249 15"}; // tag, card-original-yellow, card2
  // char *precard[] = ;
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);
      Serial.println(rfidCard);
      int i=0;
      for(;i<3;i++){
        if(rfidCard == oldcards[i]){
          Serial.println("Welcome, user!");
          lcd.clear();
          lcd.print("Welcome, user!");
          buzz();
          opendoor();
          break;
        }
      }
      if(i>=3){
        if(rfidCard==blacklist[0]){
          lcd.clear();
          Serial.println("ALERT! BLACKLISTED USER!");
          lcd.print("STOLEN CARD!");
          buzz2();
        }else{
        lcd.clear();
        Serial.println("ALERT! UNKNOWN USER!");
        lcd.print("INVALID CARD!");
        buzz();
        }
      }
      // precard.append(rfidcard);
    }
    rfid.halt();
  }
}