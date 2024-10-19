#include <avr/sleep.h>
#define _STR(X) #X
#define STR(X) _STR(X)
#define VSYNC_PIN 11
#define HSYNC_PIN 9
#define RED_PIN1 24
#define RED_PIN2 25
#define GREEN_PIN1 26
#define GREEN_PIN2 27
#define BLUE_PIN1 28
#define BLUE_PIN2 29
#define DISPLAY_PIN 4
#define VERTICAL_SKIP 90
#define RES_WIDTH 640
#define RES_HEIGHT 480
#define PIXELS_WIDTH 120
#define PIXELS_HEIGHT 60
byte displayBuffer[PIXELS_WIDTH * PIXELS_HEIGHT]; 
#define IMAGE_WIDTH 1920
#define IMAGE_HEIGHT 1080
__enable_irq();
#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin

int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis

void setup() {
  pinMode(7, INPUT);
  digitalWrite(7, LOW);
  Serial.begin(9600) ;
}

int read(){
   while(true){
    xValue = analogRead(VRX_PIN);
    yValue = analogRead(VRY_PIN);
    if(digitalRead(4)==0){
      if(xValue==0){
        // up
        return 0;
      }
      if(xValue>=900){
        // down
        return 1;
      }
      if(yValue<=20){
        // right
        return 2;
      }
      if(yValue>=900){
        // left
        return 3;
      }
    }else{
      if(xValue==0){
        // drag up
        return 5;
      }
      if(xValue>=900){
        // drag down
        return 6;
      }
      if(yValue<=20){
        // drag right
        return 7;
      }
      if(yValue>=900){
        // drag left
        return 8;
      }
      // click
      return 4;
    }
  }
}

void loop() {
  // read analog X and Y analog values
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  // print data to Serial Monitor on Arduino IDE
  //int val = read();
  //Serial.println(val);
  Serial.println(digitalRead(7));
  delay(200);
}