#include <Encoder.h>
#include <WS2812FX.h>
#include "LedPrint.h"


#define LED_COUNT 288
#define LED_PIN 6

#define TIMER_MS 60000

LedPrint myLed = LedPrint
      (
        4, // DATA PIN
        8, // CLOCK PIN
        7, // CS PIN
        8,  // NUMBER OF DIGITS
        1   // Orientation 0/1, if it looks backwards try the other
      );
    
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
Encoder encoder(2, 9);
int counter = 0;
const byte interruptPin = 3;

unsigned long last_change = 0;
unsigned long now = 0;


void setup() {
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changeMode, HIGH);
  ws2812fx.init();
  ws2812fx.setBrightness(255);
  ws2812fx.setSpeed(700);
  ws2812fx.setColor(0x8bff84);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();
}

void loop() {
  now = millis();
  ws2812fx.service();
  counter = encoder.read();
  if(counter > 0)
    counter /= 2;

  if(counter >= 0) {
    myLed.println(String(counter));
  }else{
    myLed.println("Auto");
    counter = -1;
    encoder.write(-1);
    if(now - last_change > TIMER_MS) {
      ws2812fx.setMode((ws2812fx.getMode() + 1) % ws2812fx.getModeCount());
      last_change = now;
    }
  }
}

void changeMode() {
  if(counter < 0) {
    last_change = now + TIMER_MS +1 ;
  }
  ws2812fx.setMode(counter % ws2812fx.getModeCount());
}
