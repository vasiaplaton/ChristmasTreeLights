// ****************************** ОГОНЁК ******************************
#define SPACE 2
#define TRAIN_AMOUNT 7

CRGB train_colors[TRAIN_AMOUNT];

void train() {
  if (loadingFlag) {
    loadingFlag = false;
    for (int i = 0; i < TRAIN_AMOUNT; i++) train_colors[i] = CHSV(random(0, 25) * 10, 255, 255);
  }
  fillAll(CRGB::Black);
  if (direction) {
    led_now++;
    if (led_now > NUM_LEDS - 2 - SPACE * TRAIN_AMOUNT) {
      direction = false;
    }
  } else {
    led_now--;
    if (led_now < 1) {
      direction = true;
      loadingFlag = true;
    }
  }
  for (int i = 0; i < TRAIN_AMOUNT; i++) {
    leds[led_now + i * SPACE] = train_colors[i];
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
    leds[pos[i]] = bugColors[i];
  }
}

// ****************************** ЦВЕТА ******************************
void colors() {
  if (loadingFlag) loadingFlag = false;
  hue += 1;
  CRGB thisColor = CHSV(hue, 255, 255);
  for (int i = 0; i < NUM_LEDS; i++) {
    if ( i % 4 == 0) {
      leds[i + led_now] = thisColor;
    }
    else {
      leds[i + led_now] = CRGB::Black;
    }
  }
  if (a % 8)led_now++;
  if (led_now > 3) led_now = 0;
}
// ****************************** РАДУГА ******************************
#define NUM_LEDS_R 24
void rainbow() {
  if (loadingFlag) loadingFlag = false;
  fillAll(CRGB::Black);
  hue += 2;
  for (int i = 0; i <= NUM_LEDS_R; i++)
    leds[i] = CHSV((byte)(hue + i * float(255 / NUM_LEDS_R)), 255, 255);
  for (int i = NUM_LEDS_R; i < NUM_LEDS; i++)
    leds[i] = leds[i % NUM_LEDS_R];
}

// ****************************** КОНФЕТТИ ******************************
void sparkles() {
  if (loadingFlag) {
    loadingFlag = false;
    fillAll(CRGB::Black);
    counter = 0;
  }
  if (direction) {
    byte thisNum = random(0, NUM_LEDS);
    while (getPixColor(thisNum) != 0) thisNum = random(0, NUM_LEDS);
    counter++;
    leds[thisNum] = CHSV(random(0, 255), 255, 255);
    if (counter >= NUM_LEDS - 1) {
      counter = 0;
      direction = false;
    }
  }
  else {
    byte thisNum = random(0, NUM_LEDS);
    while (getPixColor(thisNum) == 0) thisNum = random(0, NUM_LEDS);
    counter++;
    leds[thisNum] = CRGB::Black;
    if (counter >= NUM_LEDS - 1) {
      counter = 0;
      direction = true;
    }
  }
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
  if (loadingFlag) loadingFlag = false;
  int val;
  aa++;
  if ((aa % 5) == 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
      zoneRndValues[i] = random(0, 10);
    }
  }
  int thisPos = 0, lastPos = 0;
  for (int i = 0; i < NUM_LEDS / 6; i++) {
    zoneValues[i] = (float)zoneValues[i] * (1 - SMOOTH_K) + (float)zoneRndValues[i] * 10 * SMOOTH_K;
    val = zoneValues[i];
    CHSV color = CHSV(HUE_START + map(val, 20, 60, 0, HUE_GAP), constrain(map(val, 20, 60, MAX_SAT, MIN_SAT), 0, 255), constrain(map(val, 20, 60, MIN_BRIGHT, MAX_BRIGHT), 0, 255));
    leds[i] = color;
  }
  for (int i = 13; i < NUM_LEDS; i++) {
    leds[i] = leds[i % 12];
  }
  FastLED.show();
}

void filling() {
  if (loadingFlag) {
    loadingFlag = false;
    fillAll(CRGB::Black);
  }
  leds[led_now] = direction ? CHSV(hue, 255, 255) : CHSV(0, 0, 0);
  leds[led_now + 1] = direction ? CHSV(hue, 255, 255) : CHSV(0, 0, 0);
  leds[led_now + 2] = direction ? CHSV(hue, 255, 255) : CHSV(0, 0, 0);
  led_now += 3;
  if (led_now >= NUM_LEDS - 1) {
    led_now = 0;
    direction = !direction;
    hue += 20;
  }
}

void snow() {
  if (loadingFlag) {
    loadingFlag = false;
    fillAll(CRGB::Black);
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((uint32_t)getPixColor(i) == 0) continue;
    leds[i].nscale8(80);
  }
  for (int i = 0; i <= 3; i++) {
    byte thisNum = random(0, NUM_LEDS);
    while (getPixColor(thisNum) != 0) thisNum = random(0, NUM_LEDS);
    leds[thisNum] = CHSV(148, 155, 204);
  }
  //fillAll(CHSV(148, 155, 204));
  //delay(10);
}
uint8_t HUE_VAL[NUM_LEDS / 4 + 1];
void slow_rainbow() {
  if (loadingFlag) {
    loadingFlag = false;
    fillAll(CRGB::Black);
    hue = 0;
    counter = 0;
    for (int i = 0; i < NUM_LEDS / 4 + 1; i++) {
      HUE_VAL[i] = i * 30;
    }
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    if ( (i % 4) == 0 ) {
      leds[i] = CHSV(HUE_VAL[i / 4], 255, 255);
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
    fillAll(CRGB::Black);
    for (int i = 0; i < NUM_LEDS; i++) {
      if ( (i % 4) == 0 ) {
        leds[i] = CRGB::White;
      }
    }
    counter = 0;
    led_now = 0;
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    if ( (i % 4) == 0 ) {
      switch (counter) {
        case 0:
          leds[i + led_now].r -= SPEED_STD_LIGHTS;
          leds[i + led_now].g -= SPEED_STD_LIGHTS;
          leds[i + led_now].b -= SPEED_STD_LIGHTS;
          if (leds[i + led_now].r < SPEED_STD_LIGHTS) {
            counter = 1;
            fillAll(CRGB::Black);
          }
          break;
        case 1:
          led_now = 1;
          leds[i + led_now].g += SPEED_STD_LIGHTS;
          if (leds[i + led_now].g > 255 - SPEED_STD_LIGHTS) counter = 2;
          break;
        case 2:
          leds[i + led_now].g -= SPEED_STD_LIGHTS;
          if (leds[i + led_now].g < SPEED_STD_LIGHTS) {
            counter = 3;
            fillAll(CRGB::Black);
          }
          break;
        case 3:
          led_now = 2;
          leds[i + led_now].r += SPEED_STD_LIGHTS;
          if (leds[i + led_now].r > 255 - SPEED_STD_LIGHTS) counter = 4;
          break;
        case 4:
          leds[i + led_now].r -= SPEED_STD_LIGHTS;
          if (leds[i + led_now].r < SPEED_STD_LIGHTS) {
            counter = 5;
            fillAll(CRGB::Black);
          }
          break;
        case 5:
          led_now = 3;
          leds[i + led_now].b += SPEED_STD_LIGHTS;
          if (leds[i + led_now].b > 255 - SPEED_STD_LIGHTS) counter = 6;
          break;
        case 6:
          leds[i + led_now].b -= SPEED_STD_LIGHTS;
          if (leds[i + led_now].b < SPEED_STD_LIGHTS) {
            counter = 7;
            fillAll(CRGB::Black);
          }
          break;
        case 7:
          led_now = 0;
          leds[i + led_now].r += SPEED_STD_LIGHTS;
          leds[i + led_now].g += SPEED_STD_LIGHTS;
          leds[i + led_now].b += SPEED_STD_LIGHTS;
          if (leds[i + led_now].b > 255 - SPEED_STD_LIGHTS) counter = 0;
      }
    }
  }
}

// ****************** СЛУЖЕБНЫЕ ФУНКЦИИ *******************
void fade() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((uint32_t)getPixColor(i) == 0) continue;
    leds[i].fadeToBlackBy(TRACK_STEP);
  }
}
