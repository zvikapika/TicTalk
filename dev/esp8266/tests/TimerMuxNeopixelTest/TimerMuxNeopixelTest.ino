#include <Adafruit_NeoPixel.h>

#define SENSOR_COUNT 4
#define SAMPLE_INTERVAL_MS 2
#define RUNTIME_SAMPLE_COUNT 20
#define BASELINE_SAMPLE_COUNT 500
#define LIGHT_THRESHOLD 5

const int channelControlPins[] = {D7, D6, D5, D0};
const int enablePin = D8;
const int inputPin = A0;
const int overflowIndicationPin = D4; // builtin led
const int neopixelPin = D3;

const int pixelCount = 32;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neopixelPin, NEO_GRB + NEO_KHZ800);
int sampleCounter = 0;

extern "C" {
#include "user_interface.h"
}

os_timer_t myTimer;
volatile boolean dataAvailable;
volatile boolean overflowDetected;

int inputs[SENSOR_COUNT] = {0};
long acc[SENSOR_COUNT] = {0};
long baselines[SENSOR_COUNT] = {0};
int startInput = 12;
int endInput = 15;

void setup() {
  Serial.begin(500000);
  Serial.println("\n\n");
  pinMode(overflowIndicationPin, OUTPUT);
  digitalWrite(overflowIndicationPin, HIGH);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW); // 4067 is active LOW
  for (int i = 0; i < 4; ++i) {
    pinMode(channelControlPins[i], OUTPUT);
  }
  Serial.println("calibrating...");
  for (int s = 0; s < BASELINE_SAMPLE_COUNT; ++s) {
    collect();
    for (int i = 0; i < SENSOR_COUNT; ++i) {
      acc[i] += inputs[i];
    }
  }
  Serial.println("baselines:");
  for (int i = 0; i < SENSOR_COUNT; ++i) {
    baselines[i] = acc[i] / BASELINE_SAMPLE_COUNT;
    Serial.print(baselines[i]); Serial.print(' ');
  }
  strip.begin();
  strip.show();
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 0, 0), 50); // Red
  user_init();
}

void user_init(void) {
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, SAMPLE_INTERVAL_MS, true);
}

void timerCallback(void *pArg) {
  digitalWrite(overflowIndicationPin, !dataAvailable);
  collect();
  dataAvailable = true;
}

void loop() {
  if (dataAvailable) {
    for (int i = 0; i < SENSOR_COUNT; ++i) {
      acc[i] += (abs(inputs[i] - baselines[i]));
    }
    sampleCounter++;
    if (sampleCounter == RUNTIME_SAMPLE_COUNT) {
      updateLed();
      sampleCounter = 0;
      for (int i = 0; i < SENSOR_COUNT; ++i) {
        acc[i] = 0;
      }
    }
    dataAvailable = false;
  }
}

void collect() {
  int i = 0;
  for (int muxChannel = startInput; muxChannel <= endInput; muxChannel++) {
    setInputChannel(muxChannel);
    inputs[i++] = analogRead(inputPin);
  }
}

void setInputChannel(int whichChannel) {
  for (int inputPin = 0; inputPin < 4; inputPin++) {
    int pinState = bitRead(whichChannel, inputPin);
    digitalWrite(channelControlPins[inputPin], pinState);
  }
}

void updateLed() {
  int ledsPerSensor = (pixelCount / SENSOR_COUNT);
  for (int i = 0; i < SENSOR_COUNT; ++i) {
    acc[i] = (acc[i] / RUNTIME_SAMPLE_COUNT);
    Serial.print(acc[i]); Serial.print(' ');
    long col = (acc[i] > LIGHT_THRESHOLD) ? getColor(i, acc[i]) : 0;
    for (int p = i * ledsPerSensor ; p < (i+1) * ledsPerSensor; ++p) {
      strip.setPixelColor(p, col);
    }
  }
  strip.show();
  Serial.println();
}

long getColor(int sensorIdx, long volume) {
  switch (sensorIdx) {
    case 0:
      return strip.Color(volume * 2, 0, 0);
      break;
    case 1:
      return strip.Color(0, volume * 2, 0);
      break;
    case 2:
      return strip.Color(0, 0, volume * 2);
      break;
    case 3:
      return strip.Color(volume * 2, volume * 2, 0);
      break;
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

