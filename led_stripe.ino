#include <Encoder.h>
#include <WS2812FX.h>
#include <SevenSegmentTM1637.h>


#define LED_COUNT 288
#define LED_PIN 6

#define TIMER_MS 60000

SevenSegmentTM1637 display(8, 4);

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
Encoder encoder(2, 9);
int counter = 0;
int oldCounter = counter;
const byte interruptPin = 3;

unsigned long last_change = 0;
unsigned long now = 0;


void setup() {
  display.begin();
  display.setBacklight(100);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changeMode, HIGH);
  ws2812fx.init();
  ws2812fx.setBrightness(255);
  ws2812fx.setSpeed(700);
  ws2812fx.setColor(0x8bff84);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();
  counter = -1;
  display.print("Auto");
}

void loop() {
  now = millis();
  ws2812fx.service();
  counter = encoder.read();
  if(counter > 0)
    counter /= 2;

  if(counter >= 0) {
    if(oldCounter!=counter) {
      display.print(String(counter));
      oldCounter = counter;    
    }
  }else{
    if(oldCounter!=counter) {
      display.print("Auto");
      oldCounter = counter;    
    }
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
