#include <FastLED.h>
#include "GyverButton.h"
#include "GyverTimer.h"
#include <EEPROM.h>
// settings
#define TRACK_STEP 3 // less - more steps in animation
#define FIRE_PALETTE 0  // types of fire
#define AUTOPLAY_TIME 60 // time of change mode
#define NUM_LEDS 144 // number of leds
#define NUM_LEDS1 NUM_LEDS/2 // 1/2 number of leds for fire
#define DATA_PIN 7 // pin of ws2812b 
#define BRIGHTNESS 30 // std brightness in start
#define MIN_BRIGHTNESS 2 // min brightness for hand setting
#define MODES_AMOUNT 10 // number of modes
#define STD_SPEED 1.5 // speed of animation bigger - faster

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
boolean autoplay = 1; // autoplay start state
boolean slow_mode = false;
int brightness = BRIGHTNESS;
int mode = 1; // mode start
int a = 0;
byte counter = 0;
// counters
byte hue;
byte led_now = 0;
boolean direction = true;
int aa = 0;
void setup() {
  // add leds
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  Serial.begin(9600);
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
    if ( mode == 0 && (a % (int)(3 * STD_SPEED)) == 0) train();
    if ( mode == 1 && (a % (int)(6 * STD_SPEED)) == 0) lightBugs();
    if ( mode == 2 && (a % (int)(4 * STD_SPEED)) == 0) colors();
    if ( mode == 3 && (a % (int)(4 * STD_SPEED)) == 0) rainbow();
    if ( mode == 4 && (a % (int)(2 * STD_SPEED)) == 0) sparkles();
    if ( mode == 5 && (a % (int)(2 * 1)) == 0) fire1();
    if ( mode == 6 && (a % (int)(12 * STD_SPEED)) == 0) snow();
    if ( mode == 7 && (a % (int)(3 * STD_SPEED)) == 0) filling();
    if ( mode == 8 && (a % (int)(3 * STD_SPEED)) == 0) slow_rainbow();
    if ( mode == 9 && (a % (int)(20 * STD_SPEED)) == 0) slow_random();
    FastLED.show();
  }
}
void autoPlayTick() { // autoplay tick
  if (autoplayTimer.isReady() && autoplay) {// таймер смены режима
    mode++;
    loadingFlag = true;
    direction = true;
    led_now = 0;
    if ( mode >= MODES_AMOUNT) mode = 0;
  }
  randomSeed(analogRead(0)); // add entropy to random
}

void serialTick() {
  if (Serial.available() > 0) {
    String serial = Serial.readString();
    if (serial[0] == 's') {
      if (serial[1] == '1') slow_mode = true;
      else if (serial[1] == '0') slow_mode = false;
      else if (serial[1] == 'g') {
        if (slow_mode) {
          Serial.print("1");
        }
        else {
          Serial.print("0");
        }
      }
    }
  }
}

void loop()
{
  ModeTick();
  autoPlayTick();
  serialTick();
}
