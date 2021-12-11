#include <FastLED.h>
#include "GyverButton.h"
#include "GyverTimer.h"
#include <EEPROM.h>
// settings
#define TRACK_STEP 3 // less - more steps in animation
#define FIRE_PALETTE 0  // types of fire
#define AUTOPLAY_TIME 30 // time of change mode
#define NUM_LEDS 144 // number of leds
#define DATA_PIN 7 // pin of ws2812b 
#define BRIGHTNESS 30 // std brightness in start
#define MIN_BRIGHTNESS 2 // min brightness for hand setting
#define MODES_AMOUNT 9 // number of modes
#define MODES_AMOUNT_SLOW 3 // number of modes
#define STD_SPEED 1 // speed of animation bigger - faster
#define SLOW_SPEED 1

// end
// timer
GTimer_ms effectTimer(10); //60
GTimer_ms autoplayTimer((long)AUTOPLAY_TIME * 1000);
// variables
boolean loadingFlag = true; // need or not setup for effect
//float temp_br; // temp brightness for autobright
bool gReverseDirection = false;
boolean power = 1; // power in start
boolean autoplay = 1; // autoplay start state
boolean slow_mode = true; // mode for ambient light
int brightness = BRIGHTNESS;
int mode =  0; // mode start
int a = 0;
uint8_t counter = 0;
// counters
uint8_t hue;
byte led_now = 0;
boolean direction = true;
boolean pause = false;

//buttons
#define BUTTON_PIN 5
GButton BUTTON(BUTTON_PIN, LOW_PULL, NORM_OPEN);

void ButtonTick() {
  BUTTON.tick();
  BUTTON.setDebounce(0);
  boolean BUTTON_CL = BUTTON.isStep();
  if ( BUTTON_CL) {
    power = !power;
  }

  if (BUTTON.hasClicks()) { // UP has clicked
    byte clicks = BUTTON.getClicks();
    switch (clicks) {
      case 1: // one click
        slow_mode = !slow_mode;
        loadingFlag = true;
        mode = 0;// power off
        break;
      case 2: // two click
        pause = !pause;
        break;
      case 3: // three click
        increaseMode();
          break;
      case 4:
        autoplay = !autoplay;
        break;
    }
  }


  if (power)    FastLED.setBrightness(brightness); // power on, set normal brightness
  if (!power) FastLED.setBrightness(0); // power off, off leds
  FastLED.show();
}


class ledStrip {
  public:
    ledStrip(byte data_pin, int num_leds = 1, byte bright_get = 255) {
      this->led_amount = num_leds;
      this->bright = bright_get;
      realloc(this->leds, this->led_amount * 3);
      FastLED.addLeds<NEOPIXEL, DATA_PIN>( this->leds, this->led_amount);
      FastLED.setBrightness(bright);
    }
    void fillAll(CRGB newcolor) {
      for (int i = 0; i < this->led_amount; i++) {
        this->leds[i] = newcolor;
      }
    }
    uint32_t getPixColor(int thisPixel) {
      return (((uint32_t)this->leds[thisPixel].r << 16) | ((long)this->leds[thisPixel].g << 8 ) | (long)this->leds[thisPixel].b);
    }
    int getAmount() {
      return this->led_amount;
    }
    void setBright(byte bright_get) {
      this->bright = bright_get;
      FastLED.setBrightness(bright);
    }
    void fade() {
      for (int i = 0; i < NUM_LEDS; i++) {
        if ((uint32_t)this->getPixColor(i) == 0) continue;
        this->leds[i].fadeToBlackBy(TRACK_STEP);
      }
    }
    CRGB *leds = malloc(3);
    int led_amount = 1;
    byte bright;

};


ledStrip myLed(DATA_PIN, NUM_LEDS, BRIGHTNESS);

void setup() {
  Serial.begin(9600);
  // seed for random
  randomSeed(analogRead(0));
}

void ModeTick() { // draw mode
  if (effectTimer.isReady() && power && !pause) {
    a++;
    if (!slow_mode) {
      if ( mode == 0 && (a % (int)(3 * STD_SPEED)) == 0) train();
      if ( mode == 1 && (a % (int)(6 * STD_SPEED)) == 0) lightBugs();
      if ( mode == 2 && (a % (int)(4 * STD_SPEED)) == 0) colors();
      if ( mode == 3 && (a % (int)(4 * STD_SPEED)) == 0) rainbow();
      if ( mode == 4 && (a % (int)(2 * STD_SPEED)) == 0) sparkles();
      if ( mode == 5 && (a % (int)(2 * 1)) == 0) fire1();
      if ( mode == 6 && (a % (int)(12 * STD_SPEED)) == 0) snow();
      if ( mode == 7 && (a % (int)(3 * STD_SPEED)) == 0) filling();
      if ( mode == 8 && (a % (int)(3 * STD_SPEED)) == 0) slow_rainbow();
      //if ( mode == 9 && (a % (int)(20 * STD_SPEED)) == 0) slow_random();
    }
    else {
      if ( mode == 0 && (a % (int)(3 * SLOW_SPEED)) == 0) slow_rainbow();
      if ( mode == 1 && (a % (int)(2 * SLOW_SPEED)) == 0) std_lights();
      //if ( mode == 2 && (a % (int)(STD_SPEED)) == 0) waves();
      if ( mode == 2 && (a % (int)(STD_SPEED)) == 0) slow_sparkles();
    }
    FastLED.show();
  }
}
void autoPlayTick() { // autoplay tick
  if (autoplayTimer.isReady() && autoplay) {// таймер смены режима
    increaseMode();
  }
  randomSeed(analogRead(0)); // add entropy to random
}

void increaseMode() {
  mode++;
  loadingFlag = true;
  direction = true;
  led_now = 0;
  if (!slow_mode) {
    if ( mode >= MODES_AMOUNT) mode = 0;
  }
  else {
    if ( mode >= MODES_AMOUNT_SLOW) mode = 0;
  }
}

void serialTick() {
  if (Serial.available() > 0) {
    String serial = Serial.readString();
    if (serial[0] == 's') {
      myLed.fillAll(CRGB::Black);
      loadingFlag = true;
      if (serial[1] == '1') slow_mode = true;
      else if (serial[1] == '0') slow_mode = false;
    }
  }
}

void loop()
{
  ModeTick();
  autoPlayTick();
  serialTick();
  ButtonTick();
}
