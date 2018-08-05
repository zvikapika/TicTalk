#include <ESP8266WiFi.h>

#define SENSOR_COUNT 4
#define SAMPLE_INTERVAL_MS 2

#define DEBUG_SENSORS

const int channelControlPins[] = {D7, D6, D5, D0};
const int enablePin = D8;
const int inputPin = A0;
const int overflowIndicationPin = LED_BUILTIN;

extern "C" {
#include "user_interface.h"
}

os_timer_t myTimer;
volatile boolean dataAvailable;
volatile boolean overflowDetected;

int inputs[SENSOR_COUNT] = {0};
int startInput = 12;
int endInput = 15;

void setup() {
  Serial.begin(500000);
//  WiFi.mode(WIFI_OFF);
  pinMode(overflowIndicationPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW); // 4067 is active LOW
  for (int i = 0; i < 4; ++i) {
    pinMode(channelControlPins[i], OUTPUT);
  }
  user_init();
}


void user_init(void) {
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, SAMPLE_INTERVAL_MS, true);
}

void timerCallback(void *pArg) {
  if (dataAvailable) {
    overflowDetected = true;
  }
  collect();
  dataAvailable = true;
}

void loop() {
  if (dataAvailable) {
    if (overflowDetected) {
#ifdef DEBUG_SENSORS
      Serial.print("OVERFLOW ");
#endif      
      digitalWrite(overflowIndicationPin, LOW); // builtin led - low is on
      overflowDetected = false;
    }
    else {
      digitalWrite(overflowIndicationPin, HIGH);
    }
#ifdef DEBUG_SENSORS    
    Serial.print("0 1023 ");
    for (int i = 0; i < SENSOR_COUNT; ++i) {
      Serial.print(inputs[i]); Serial.print(' ');
    }
    Serial.println();
#endif    
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

