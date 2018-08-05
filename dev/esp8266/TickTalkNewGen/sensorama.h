#define SENSOR_COUNT 4
#define SAMPLE_INTERVAL_MS 4
#define RUNTIME_SAMPLE_COUNT 20
#define BASELINE_SAMPLE_COUNT 500
#define LIGHT_THRESHOLD 10
#define SOOTHING_FACTOR 25

const int channelControlPins[] = {D7, D6, D5, D0};
const int enablePin = D8;
const int inputPin = A0;

// red, yellow, green, blue
const float factor[SENSOR_COUNT] = {1.2, 1.05, 0.9, 1.15};
long sensorLongTermRecords[SENSOR_COUNT] = {0};

int sampleCounter = 0;

extern "C" {
#include "user_interface.h"
}

os_timer_t myTimer;
volatile boolean dataAvailable;
volatile boolean overflowDetected;

int inputs[SENSOR_COUNT] = {0,0,0,0};
long acc[SENSOR_COUNT] = {0,0,0,0};
long secondOrderAcc[SENSOR_COUNT] = {0,0,0,0};
long baselines[SENSOR_COUNT] = {0};
int startInput = 12;
int endInput = 15;
int ledsPerSensor;

void setInputChannel(int whichChannel) {
  for (int inputPin = 0; inputPin < 4; inputPin++) {
    int pinState = bitRead(whichChannel, inputPin);
    digitalWrite(channelControlPins[inputPin], pinState);
  }
}

void collect() {
  int i = 0;
  for (int muxChannel = startInput; muxChannel <= endInput; muxChannel++) {
    setInputChannel(muxChannel);
    inputs[i++] = analogRead(inputPin);
  }
}

void timerCallback(void *pArg) {
  collect();
  dataAvailable = true;
}

void user_init(void) {
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, SAMPLE_INTERVAL_MS, true);
}

void updateCounters() {
  for (int i = 0; i < SENSOR_COUNT; ++i) {
    // Serial.print(acc[i]); Serial.print(' ');
    acc[i] = (acc[i] / RUNTIME_SAMPLE_COUNT);
    secondOrderAcc[i] += (acc[i] * factor[i]);
    secondOrderAcc[i] -= SOOTHING_FACTOR;
    if(secondOrderAcc[i] < 0) secondOrderAcc[i] = 0;
    // Serial.print(acc[i]); Serial.print(' ');
    Serial.print(secondOrderAcc[i]); Serial.print(' ');
  }
  Serial.println();
}

long getColor(int sensorIdx, long volume) {
  switch (sensorIdx) {
    case 0:
      return strip.Color(volume, 0, 0);
      break;
    case 1:
      return strip.Color(volume,  volume, 0);
      break;
    case 2:
      return strip.Color(0, volume, 0);
      break;
    case 3:
      return strip.Color(0, 0, volume);
      break;
  }
}

void updateLeds() {
  long record = -1;
  int recordIdx = -1;
  for (int i = 0; i < SENSOR_COUNT; ++i) {
    if (secondOrderAcc[i] > LIGHT_THRESHOLD && secondOrderAcc[i] > record) {
      record = secondOrderAcc[i];
      recordIdx = i;
    }
  }
  if(record > 255) {
    int substract = record-255;
    for (int i = 0; i < SENSOR_COUNT; ++i) {
      secondOrderAcc[i] -= substract;
    }
    record -= substract;
  }

  for (int i = 0; i < strip.numPixels(); ++i) {
    strip.setPixelColor(i, 0);
  }
  if (record > 0) {
    long col = getColor(recordIdx, constrain(record, 0, 255));
    sensorLongTermRecords[recordIdx]++;
    for (int p = recordIdx * ledsPerSensor ; p < (recordIdx + 1) * ledsPerSensor; ++p) {
      strip.setPixelColor(p, col);
    }
  }
  strip.show();
}

void sensoramaCalibrate() {
  Serial.println("calibrating sensors...");
  for (int s = 0; s < BASELINE_SAMPLE_COUNT; ++s) {
    collect();
    for (int i = 0; i < SENSOR_COUNT; ++i) {
      acc[i] += inputs[i];
    }
  }
  Serial.println("sensor baselines:");
  for (int i = 0; i < SENSOR_COUNT; ++i) {
    baselines[i] = acc[i] / BASELINE_SAMPLE_COUNT;
    Serial.print(baselines[i]); Serial.print(' ');
    acc[i] = 0;
  }
  Serial.println();
  
}

void sensoramaSetup() {
  // WiFi.mode(WIFI_OFF);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW); // 4067 is active LOW
  for (int i = 0; i < 4; ++i) {
    pinMode(channelControlPins[i], OUTPUT);
  }
  sensoramaCalibrate();
  user_init();
  ledsPerSensor = (strip.numPixels() / SENSOR_COUNT);
}

void sensoramaReset() {
  for(int i = 0; i < SENSOR_COUNT; ++i) {
    sensorLongTermRecords[i] = 0;
  }
  // sensoramaCalibrate(); // needed?
}

void sensoramaUpdate() {
  if (dataAvailable) {
    for (int i = 0; i < SENSOR_COUNT; ++i) {
      acc[i] += (abs(inputs[i] - baselines[i]));
    }
    sampleCounter++;
    if (sampleCounter == RUNTIME_SAMPLE_COUNT) {
      updateCounters();
      updateLeds();
      sampleCounter = 0;
      for (int i = 0; i < SENSOR_COUNT; ++i) {
        acc[i] = 0;
      }
    }
    dataAvailable = false;
  }
}

