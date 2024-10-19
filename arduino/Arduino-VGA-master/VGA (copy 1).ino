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
#define IMAGE_MONA6
#include "image.h"
int blankLinesLeft;
byte line;
byte sLine;
#define SUBLINES_PER_LINE 6
void setup() {
  pinMode(VSYNC_PIN, OUTPUT);
  pinMode(HSYNC_PIN, OUTPUT);
  pinMode(RED_PIN1, OUTPUT);
  pinMode(RED_PIN2, OUTPUT);
  pinMode(GREEN_PIN1, OUTPUT);
  pinMode(GREEN_PIN2, OUTPUT);
  pinMode(BLUE_PIN1, OUTPUT);
  pinMode(BLUE_PIN2, OUTPUT);
  for(int y = 0; y < PIXELS_HEIGHT; y++){
    for(int x = 0; x < PIXELS_WIDTH; x++) {
      displayBuffer[x + y * PIXELS_WIDTH] = pgm_read_byte_near(IMAGE + (x % IMAGE_WIDTH) + ((y % IMAGE_HEIGHT) * IMAGE_WIDTH));
    }
  }
  for(int y = 0; y < PIXELS_HEIGHT; y++){
    for(int x = 0; x < PIXELS_WIDTH; x++) {
      displayBuffer[x + y * PIXELS_WIDTH] = pgm_read_byte_near(IMAGE + (x % IMAGE_WIDTH) + ((y % IMAGE_HEIGHT) * IMAGE_WIDTH));
    }
  }
  cli();
  TIMSK0=0;
  TCCR0A=0;
  TCCR0B=(1 << CS00);
  OCR0A=0;
  OCR0B=0;
  TCNT0=0;
  TCCR1A = bit(WGM11) | bit(COM1A1);
  TCCR1B = bit(WGM12) | bit(WGM13) | bit(CS12) | bit(CS10);
  ICR1 = 259;
  OCR1A = 0;
  TIFR1 = bit(TOV1);
  TIMSK1 = bit(TOIE1);
  TCCR2A = bit(WGM20) | bit(WGM21) | bit(COM2B1);
  TCCR2B = bit(WGM22) | bit(CS21);
  OCR2A = 63;
  OCR2B = 7;
  TIFR2 = bit(TOV2);
  TIMSK2 = bit(TOIE2);
  sei();
  set_sleep_mode (SLEEP_MODE_IDLE);
}
void loop() {
  sleep_mode();
}
ISR (TIMER1_OVF_vect) {
  blankLinesLeft = VERTICAL_SKIP;
  sLine = -1;
  line = 0;
}
ISR (TIMER2_OVF_vect) {
  if(blankLinesLeft) {
    blankLinesLeft--;
    return;
  }
  if(line < PIXELS_HEIGHT) {
    asm volatile(
      ".rept 30                   \n\t"
      "  nop                      \n\t"
      ".endr                      \n\t"
      ".rept " STR(PIXELS_WIDTH) "\n\t"
      "  ld r16, Z+               \n\t"
      "  out %[port], r16         \n\t"
      ".endr                      \n\t"
      "nop                        \n\t"
      "ldi r16,0                  \n\t"
      "out %[port], r16           \n\t"
      :
      : [port] "I" (_SFR_IO_ADDR(PORTA)),
        "z" "I" (displayBuffer + line * PIXELS_WIDTH)
      : "r16", "r20", "memory"
    );
    if(++sLine == SUBLINES_PER_LINE - 1) {
      sLine = -1;
      line++;
    }
  }
}
inline void setPixel(byte x, byte y, byte color) {
  displayBuffer[x + y * PIXELS_WIDTH] = color;
}
