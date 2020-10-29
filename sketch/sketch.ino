#include <FastLED.h>
#include "GyverButton.h"
#include "GyverTimer.h"
#include <EEPROM.h>
// settings
#define TRACK_STEP 1 // less - more steps in animation
#define FIRE_PALETTE 0  // types of fire
#define AUTOPLAY_TIME 15 // time of change mode
#define NUM_LEDS 144 // number of leds
#define NUM_LEDS1 NUM_LEDS/2 // 1/2 number of leds for fire
#define DATA_PIN 7 // pin of ws2812b 
#define BRIGHTNESS 15 // std brightness in start
#define MIN_BRIGHTNESS 2 // min brightness for hand setting
#define MODES_AMOUNT 6 // number of modes
#define STD_SPEED 8 // speed of animation bigger - slowly

// end
// leds routins
CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;
// timer
GTimer_ms effectTimer(10); //60
GTimer_ms autoplayTimer((long)AUTOPLAY_TIME * 1000);
// variables
boolean loadingFlag = true; // need or not setup for effect
boolean UP_CL; // long click UP
float temp_br; // temp brightness for autobright
boolean DOWN_CL; // long click DOWN
bool gReverseDirection = false;
boolean power = 1; // power in start
boolean autoplay = 0; // autoplay start state
int sped = STD_SPEED;
int brightness = BRIGHTNESS;
int mode = 0; // mode start
int a = 0;
byte counter = 0;


void setup() {
  // add leds
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  // seed for random
  randomSeed(analogRead(0));
  //fire palette
  gPal = HeatColors_p;
}

// fill all leds
void fillAll(CRGB newcolor) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = newcolor;
  }
}

// get pix color know his nubmer
uint32_t getPixColor(int thisPixel) {
  return (((uint32_t)leds[thisPixel].r << 16) | ((long)leds[thisPixel].g << 8 ) | (long)leds[thisPixel].b);
}

// next mode func
void Next_mode() {
  if (mode < MODES_AMOUNT - 1) mode++;
  loadingFlag = true;
  FastLED.clear();
  /// leds[1] = CRGB::Red;
}
//previous mode func
void Prev_mode() {
  if (mode > 0) mode--;
  loadingFlag = true;
  FastLED.clear();
  /// leds[1] = CRGB::Blue;
}
void ModeTick() { // draw mode
  if (effectTimer.isReady() && power) {
    a++;
    if (mode < 6 && ((a % sped) == 0)) {
      switch (mode) {
        case 1: lightBugs();
          break;
        case 2: colors();
          break;
        case 3: rainbow();
          break;
        case 4: sparkles();
          break;
      }
    }
    if ( mode == 5 && (a % 2) == 0) {
      fire1();
    }
    if ( mode == 0 && (a % 4) == 0) {
      lighter();
    }
    FastLED.show();
  }
}
void autoPlayTick() { // autoplay tick
  if (autoplayTimer.isReady() && autoplay) {// таймер смены режима
    mode++;
    if ( mode >= MODES_AMOUNT) mode = 0;
  }
}
void loop()
{
  ModeTick();
  autoPlayTick();

}
