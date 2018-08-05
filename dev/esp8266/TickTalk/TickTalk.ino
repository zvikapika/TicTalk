#define BLYNK_PRINT Serial

#include <Adafruit_NeoPixel.h>
#include <Regexp.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFiMulti.h>

#include "neopixel.h"
#include "ntp_time.h"
#include "states.h"
#include "wifi_util.h"
#include "blynk_util.h"


int state;

void setup() {
  Serial.begin(115200);
  state = STATE_START;
}

void loop() {
  switch (state) {
    case STATE_START:
      state = handleStart();
      break;
    case STATE_IDLE:
      state = handleIdle();
      break;
    case STATE_AWAITING_MEETING_START:
      state = handleAwaitingMeetingStart();
      break;
    case STATE_MEETING_IN_PROGRESS:
      state = handleMeetingInProgress();
      break;
    case STATE_MEETING_ENDED:
      state = handleMeetingEnded();
      break;
  }
}

int handleStart() {
  neopixelInit();
  wifiConnect();
  ntptimeSetup();
  blynkSetup();
  rainbowCycle(1);
  colorWipe(0, 15);
  return STATE_IDLE;
}

int handleIdle() {
  return STATE_IDLE;
}

int handleAwaitingMeetingStart() {
  return STATE_AWAITING_MEETING_START;
}

int handleMeetingInProgress() {
  return STATE_MEETING_IN_PROGRESS;
}

int handleMeetingEnded() {
  return STATE_MEETING_ENDED;
}

