#include <ESP8266WiFi.h>

#define SENSOR_COUNT 4

const int startInput = 12;
const int endInput = 15;
int inputs[SENSOR_COUNT] = {0};

const int channelControlPins[] = {D7, D6, D5, D0};
const int enablePin = D8;
const int inputPin = A0;

void setup() {
  Serial.begin(500000);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  for (int i = 0; i < 4; ++i) {
    pinMode(channelControlPins[i], OUTPUT);
  }
  //WiFi.mode(WIFI_STA);
}

long t1, t2;

void loop() {
  collect();
  for (int i = 0; i < SENSOR_COUNT; ++i) {
    Serial.print(inputs[i]); Serial.print(' ');
  }
  Serial.println(" 0 1023");
}

void loop1() {
  t1 = micros();
  collect();
  t2 = micros();
  Serial.println(t2 - t1);
}

void loop3() {
  t1 = micros();
  int i = 0;
  for (int muxChannel = startInput; muxChannel <= endInput; muxChannel++) {
    for (int inputPin = 0; inputPin < 4; inputPin++) {
      int pinState = bitRead(muxChannel, inputPin);
      digitalWrite(channelControlPins[inputPin], pinState);
    }
    inputs[i++] = analogRead(inputPin);
  }
  t2 = micros();
  Serial.println(t2 - t1);
}

void loop4() {
  t1 = micros();
  int i = 0;
  for (int muxChannel = startInput; muxChannel <= endInput; muxChannel++) {
    for (int inputPin = 0; inputPin < 4; inputPin++) {
      int pinState = muxChannel & (1<<inputPin);
      digitalWrite(channelControlPins[inputPin], pinState);
    }
    inputs[i++] = analogRead(inputPin);
  }
  t2 = micros();
  Serial.println(t2 - t1);
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



