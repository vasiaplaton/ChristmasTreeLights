class effect {
  public:
    effect();
};



// ****************************** ОГОНЁК ******************************
#define SPACE 2
#define TRAIN_AMOUNT 7

CRGB train_colors[TRAIN_AMOUNT];

void train() {
  if (loadingFlag) {
    loadingFlag = false;
    for (int i = 0; i < TRAIN_AMOUNT; i++) train_colors[i] = CHSV(random(0, 25) * 10, 255, 255);
  }
  myLed.fillAll(CRGB::Black);
  if (direction) {
    ledNow++;
    if (ledNow > NUM_LEDS - 2 - SPACE * TRAIN_AMOUNT) {
      direction = false;
    }
  } else {
    ledNow--;
    if (ledNow < 1) {
      direction = true;
      loadingFlag = true;
    }
  }
  for (int i = 0; i < TRAIN_AMOUNT; i++) {
    myLed.leds[ledNow + i * SPACE] = train_colors[i];
  }
}

// ****************************** СВЕТЛЯЧКИ ******************************
#define MAX_SPEED 30
#define BUGS_AMOUNT NUM_LEDS/3
int16_t speed[BUGS_AMOUNT];
int16_t pos[BUGS_AMOUNT];
CRGB bugColors[BUGS_AMOUNT];

void lightBugs() {
  if (loadingFlag) {
    loadingFlag = false;
    for (int i = 0; i < BUGS_AMOUNT; i++) {
      bugColors[i] = CHSV(random(0, 9) * 28, 255, 255);
      pos[i] = random(0, NUM_LEDS);
      speed[i] += random(-5, 6);
    }
  }
  FastLED.clear();
  for (int i = 0; i < BUGS_AMOUNT; i++) {
    speed[i] += random(-5, 6);
    if (speed[i] == 0) speed[i] += (-5, 6);

    if (abs(speed[i]) > MAX_SPEED) speed[i] = 0;
    pos[i] += speed[i] / 10;
    if (pos[i] < 0) {
      pos[i] = 0;
      speed[i] = -speed[i];
    }
    if (pos[i] > NUM_LEDS - 1) {
      pos[i] = NUM_LEDS - 1;
      speed[i] = -speed[i];
    }
    myLed.leds[pos[i]] = bugColors[i];
  }
}


// ****************************** ЦВЕТА ******************************
void colors() {
  if (loadingFlag) {
    loadingFlag = false;
    myLed.fillAll(CRGB::Black);
  }
  hue += 1;
  CRGB thisColor = CHSV(hue, 255, 255);
  for (int i = 0; i < NUM_LEDS; i++) {
    if ( i % 4 == 0) {
      myLed.leds[i + ledNow] = thisColor;
    }
    else {
      myLed.leds[i + ledNow] = CRGB::Black;
    }
  }
  if (a % 8)ledNow++;
  if (ledNow > 3) ledNow = 0;
}

void waves() {
  for (int i = 0; i < NUM_LEDS; i++) {
    int sin_now = abs(255 * sin(radians(i * 30 + ledNow * 10)));
    myLed.leds[i] = CHSV(hue, 255, sin_now);
  }
  ledNow++;
  if (a % 50 == 0) hue += 143;
}

void slow_sparkles() {
  myLed.fillAll(CRGB::Black);
  for (int i = 0; i < NUM_LEDS / 3; i++) {
    myLed.leds[i * 3 + ledNow] = CHSV(i * 143 + hue, 255, 255);
  }
  if (a % 2 == 0) hue += 1;
  //if(a % 8 == 0){
  //  led_now++;
  //  if(led_now > 3) led_now = 0;
  //}
}
// ****************************** РАДУГА ******************************
#define NUM_LEDS_R 24
void rainbow() {
  if (loadingFlag) loadingFlag = false;
  myLed.fillAll(CRGB::Black);
  hue += 2;
  for (int i = 0; i <= NUM_LEDS_R; i++)
    myLed.leds[i] = CHSV((byte)(hue + i * float(255 / NUM_LEDS_R)), 255, 255);
  for (int i = NUM_LEDS_R; i < NUM_LEDS; i++)
    myLed.leds[i] = myLed.leds[i % NUM_LEDS_R];
}

// ****************************** КОНФЕТТИ ******************************
void sparkles() {
  if (loadingFlag) {
    loadingFlag = false;
    myLed.fillAll(CRGB::Black);
    counter = 0;
  }
  if (direction) {
    byte thisNum = random(0, NUM_LEDS);
    while (myLed.getPixColor(thisNum) != 0) thisNum = random(0, NUM_LEDS);
    counter++;
    myLed.leds[thisNum] = CHSV(random(0, 255), 255, 255);
    if (counter >= NUM_LEDS - 1) {
      counter = 0;
      direction = false;
    }
  }
  else {
    byte thisNum = random(0, NUM_LEDS);
    while (myLed.getPixColor(thisNum) == 0) thisNum = random(0, NUM_LEDS);
    counter++;
    myLed.leds[thisNum] = CRGB::Black;
    if (counter >= NUM_LEDS - 1) {
      counter = 0;
      direction = true;
    }
  }
}

void sparkles1() {
    if (loadingFlag) {
    loadingFlag = false;
    myLed.fillAll(CRGB::Black);
    counter = 0;
  }
  randomSeed(analogRead(0));
  //if (myLed.getPixColor(thisNum) == 0)
  byte thisNum = random(0, NUM_LEDS);
  while (myLed.getPixColor(thisNum) != 0) {
    thisNum = random(0, NUM_LEDS);
  }
  myLed.leds[thisNum] = CHSV(random(0, 255), 255, 255);
  myLed.fade(20);
}


#define HUE_START 3     // начальный цвет огня (0 красный, 80 зелёный, 140 молния, 190 розовый)
#define HUE_GAP 18      // коэффициент цвета огня (чем больше - тем дальше заброс по цвету)
#define SMOOTH_K 0.15   // коэффициент плавности огня
#define MIN_BRIGHT 80   // мин. яркость огня
#define MAX_BRIGHT 255  // макс. яркость огня
#define MIN_SAT 245     // мин. насыщенность
#define MAX_SAT 255     // макс. насыщенность

// для разработчиков
#define ZONE_AMOUNT NUM_LEDS   // количество зон
byte zoneValues[ZONE_AMOUNT];
byte zoneRndValues[ZONE_AMOUNT];

#define ORDER_GRB       // порядок цветов ORDER_GRB / ORDER_RGB / ORDER_BRG
#define COLOR_DEBTH 3   // цветовая глубина: 1, 2, 3 (в байтах)

void fire1() {
  if (loadingFlag) {
    loadingFlag = false;
    myLed.fillAll(CRGB::Black);
    counter = 0;
  }
  int val;
  counter++;
  if ((counter % 5) == 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
      zoneRndValues[i] = random(0, 10);
    }
  }
  int thisPos = 0, lastPos = 0;
  for (int i = 0; i < NUM_LEDS / 6; i++) {
    zoneValues[i] = (float)zoneValues[i] * (1 - SMOOTH_K) + (float)zoneRndValues[i] * 10 * SMOOTH_K;
    val = zoneValues[i];
    CHSV color = CHSV(HUE_START + map(val, 20, 60, 0, HUE_GAP), constrain(map(val, 20, 60, MAX_SAT, MIN_SAT), 0, 255), constrain(map(val, 20, 60, MIN_BRIGHT, MAX_BRIGHT), 0, 255));
    myLed.leds[i] = color;
  }
  for (int i = 13; i < NUM_LEDS; i++) {
    myLed.leds[i] = myLed.leds[i % 12];
  }
  FastLED.show();
}

void filling() {
  if (loadingFlag) {
    loadingFlag = false;
    myLed.fillAll(CRGB::Black);
  }
  myLed.leds[ledNow] = direction ? CHSV(hue, 255, 255) : CHSV(0, 0, 0);
  myLed.leds[ledNow + 1] = direction ? CHSV(hue, 255, 255) : CHSV(0, 0, 0);
  myLed.leds[ledNow + 2] = direction ? CHSV(hue, 255, 255) : CHSV(0, 0, 0);
  ledNow += 3;
  if (ledNow >= NUM_LEDS - 1) {
    ledNow = 0;
    direction = !direction;
    hue += 20;
  }
}

void snow() {
  if (loadingFlag) {
    loadingFlag = false;
    myLed.fillAll(CRGB::Black);
  }
  byte thisNum = random(0, NUM_LEDS);
  while (myLed.getPixColor(thisNum) == 0) {
    myLed.leds[thisNum] = CRGB::White;
    thisNum = random(0, NUM_LEDS);
  }
  myLed.fade(10);
}
uint8_t HUE_VAL[NUM_LEDS / 4 + 1];
void slow_rainbow() {
  if (loadingFlag) {
    loadingFlag = false;
    myLed.fillAll(CRGB::Black);
    hue = 0;
    counter = 0;
    for (int i = 0; i < NUM_LEDS / 4 + 1; i++) {
      HUE_VAL[i] = i * 30;
    }
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    if ( (i % 4) == 0 ) {
      myLed.leds[i] = CHSV(HUE_VAL[i / 4], 255, 255);
    }
  }
  for (int i = 0; i < NUM_LEDS / 4 + 1; i++) {
    HUE_VAL[i] += 2;
  }
}
#define SPEED_STD_LIGHTS 3
void std_lights() {
  if (loadingFlag) {
    loadingFlag = false;
    myLed.fillAll(CRGB::Black);
    for (int i = 0; i < NUM_LEDS; i++) {
      if ( (i % 4) == 0 ) {
        myLed.leds[i + ledNow].r -= 255 / 3;
        myLed.leds[i + ledNow].g -= 255 / 3;
        myLed.leds[i + ledNow].b -= 255 / 3;
      }
    }
    counter = 0;
    ledNow = 0;
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    if ( (i % 4) == 0 ) {
      switch (counter) {
        case 0:
          myLed.leds[i + ledNow].r -= SPEED_STD_LIGHTS / 3;
          myLed.leds[i + ledNow].g -= SPEED_STD_LIGHTS / 3;
          myLed.leds[i + ledNow].b -= SPEED_STD_LIGHTS / 3;
          if (myLed.leds[i + ledNow].r < SPEED_STD_LIGHTS) {
            counter = 1;
            myLed.fillAll(CRGB::Black);
          }
          break;
        case 1:
          ledNow = 1;
          myLed.leds[i + ledNow].g += SPEED_STD_LIGHTS;
          if (myLed.leds[i + ledNow].g > 255 - SPEED_STD_LIGHTS) counter = 2;
          break;
        case 2:
          myLed.leds[i + ledNow].g -= SPEED_STD_LIGHTS;
          if (myLed.leds[i + ledNow].g < SPEED_STD_LIGHTS) {
            counter = 3;
            myLed.fillAll(CRGB::Black);
          }
          break;
        case 3:
          ledNow = 2;
          myLed.leds[i + ledNow].r += SPEED_STD_LIGHTS;
          if (myLed.leds[i + ledNow].r > 255 - SPEED_STD_LIGHTS) counter = 4;
          break;
        case 4:
          myLed.leds[i + ledNow].r -= SPEED_STD_LIGHTS;
          if (myLed.leds[i + ledNow].r < SPEED_STD_LIGHTS) {
            counter = 5;
            myLed.fillAll(CRGB::Black);
          }
          break;
        case 5:
          ledNow = 3;
          myLed.leds[i + ledNow].b += SPEED_STD_LIGHTS;
          if (myLed.leds[i + ledNow].b > 255 - SPEED_STD_LIGHTS) counter = 6;
          break;
        case 6:
          myLed.leds[i + ledNow].b -= SPEED_STD_LIGHTS;
          if (myLed.leds[i + ledNow].b < SPEED_STD_LIGHTS) {
            counter = 7;
            myLed.fillAll(CRGB::Black);
          }
          break;
        case 7:
          ledNow = 0;
          myLed.leds[i + ledNow].r += SPEED_STD_LIGHTS / 3;
          myLed.leds[i + ledNow].g += SPEED_STD_LIGHTS / 3;
          myLed.leds[i + ledNow].b += SPEED_STD_LIGHTS / 3;
          if (myLed.leds[i + ledNow].b > (255 - SPEED_STD_LIGHTS) / 3) counter = 0;
      }
    }
  }
}
