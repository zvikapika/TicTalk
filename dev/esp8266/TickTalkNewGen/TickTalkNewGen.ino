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
#include "sensorama.h"
#include "ifttt_util.h"

#define IFTTT_TOKEN "pugCQ75bLBaZEZxrjEsIXvAzrqfhNJQKgFOSaOMGN4z"
#define IFTTT_EVENT_NAME "ticktalk_update"

int state;
int nextMeetingStartDay;
int nextMeetingStartMonth;
int nextMeetingStartYear;
int nextMeetingStartMinute;
int nextMeetingDuration;
int nextMeetingTriggerMinute;
char* colorNames[SENSOR_COUNT] = {
  "Red","Yellow","Green","Blue"
};
unsigned long meetingStartedTstamp = 0;

void setup() {
  Serial.begin(500000);
  state = STATE_STARTING;
}

void loop() {
  yield();
  // Serial.print("State: "); Serial.println(state);
  switch (state) {
    case STATE_STARTING:
      state = handleStarting();
      break;
    case STATE_IDLE:
      state = handleIdle();
      break;
    case STATE_TRIGGERED:
      state = handleTriggered();
      break;
    case STATE_COUNTDOWN:
      state = handleCountdown();
      break;
    case STATE_MEETING_START:
      state = handleMeetingStart();
      break;
    case STATE_MEETING_IN_PROGRESS:
      state = handleMeetingInProgress();
      break;
    case STATE_MEETING_END:
      state = handleMeetingEnd();
      break;
    default:
      Serial.println("UNHANDLED STATE. ABORTING");
      while (1) {
        yield();
      }
  }
  if (state != STATE_STARTING) {
    Blynk.run();
  }

}

int handleStarting() {
  neopixelInit();
  wifiConnect();
  ntptimeSetup();
  blynkSetup();
  rainbowCycle(1);
  colorWipe(0, 30);
  sensoramaSetup();
  return STATE_IDLE;
}

int handleIdle() {
  return STATE_IDLE;
}

int handleTriggered() {
  Serial.println("Meeting triggered!");
  struct tm * timeinfo = ntptimeGet();
  nextMeetingTriggerMinute = timeinfo->tm_min;
  return STATE_COUNTDOWN;
}

int handleCountdown() {
  int totalCountdownMinutes = getMinuteDiff(nextMeetingStartMinute, nextMeetingTriggerMinute);
  // Serial.print("totalCountdownMinutes="); Serial.println(totalCountdownMinutes);
  // handle hour cross i.e. when trigger time is on hour X and start time is x+1
  struct tm * timeinfo = ntptimeGet();
  // Serial.println(timeinfo->tm_min);
  // Serial.println(nextMeetingTriggerMinute);
  int countdownMinutesLapsed = getMinuteDiff(timeinfo->tm_min, nextMeetingTriggerMinute);
  // Serial.print("countdownMinutesLapsed="); Serial.println(countdownMinutesLapsed);
  int pixelCount = map(countdownMinutesLapsed, 0, totalCountdownMinutes, 1, strip.numPixels());
  // Serial.print("pixelCount="); Serial.println(pixelCount);
  neopixelFadeInOut(pixelCount, strip.Color(0,255,0));
  if (timeinfo->tm_min != nextMeetingStartMinute) {
    return STATE_COUNTDOWN;
  }
  else {
    return STATE_MEETING_START;
  }
}

int handleMeetingStart() {
  Serial.println("Meeting started!");
  rainbowCycle(1);
  colorWipe(0, 30);
  sensoramaReset();
  meetingStartedTstamp = millis();
  return STATE_MEETING_IN_PROGRESS;
}

int handleMeetingInProgress() {
  sensoramaUpdate();
  if((millis() - meetingStartedTstamp)/1000/60 < nextMeetingDuration) {
    return STATE_MEETING_IN_PROGRESS;
  }
  else return STATE_MEETING_END;
}

char stats[64];

int handleMeetingEnd() {
  Serial.println("Meeting ended!");
  Serial.println("Final Stats\n===========");
  String param = "";
  for(int i = 0; i < 4; ++i) {
    // >>> remove me! only for testing
    sensorLongTermRecords[i] = random(1, 1000);
    Serial.print(colorNames[i]); Serial.print(":"); Serial.println(sensorLongTermRecords[i]);
    param += sensorLongTermRecords[i];
    if(i < 3) param += ",";
  }
  param.toCharArray(stats, min(int(param.length()), 64));
  ifttt_trigger(IFTTT_EVENT_NAME, IFTTT_TOKEN, stats, NULL, NULL);
  for(int i = 0; i < 10; ++i) {
    neopixelTheaterChase(Wheel(i * 25), 1);
    Blynk.run();
  }
  return STATE_IDLE;
}


void onAppointmentTriggered(int startDay, int startMonth, int startYear, int startHour, int startMinute, int endHour, int endMinute, int duration) {
  Serial.println("NEW SCHEDULE");
  Serial.print("startDay=");    Serial.println(startDay);
  Serial.print("startMonth=");  Serial.println(startMonth);
  Serial.print("startYear=");   Serial.println(startYear);
  Serial.print("startHour="); Serial.println( startHour);
  Serial.print("startMinute="); Serial.println( startMinute);
  Serial.print("endHour=");     Serial.println(endHour);
  Serial.print("endMinute=");   Serial.println(endMinute);
  Serial.print("appointment duration(min): "); Serial.println(duration);
  nextMeetingStartDay = startDay;
  nextMeetingStartMonth = startMonth;
  nextMeetingStartYear = startYear;
  nextMeetingStartMinute = startMinute;
  nextMeetingDuration = duration;
  state = STATE_TRIGGERED;
}

