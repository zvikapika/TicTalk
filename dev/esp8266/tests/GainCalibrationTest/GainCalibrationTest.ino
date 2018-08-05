#define SENSOR_COUNT 4
#define SAMPLE_INTERVAL_MS 2

#define DEBUG_SENSORS

const int channelControlPins[] = {D7, D6, D5, D0};
const int enablePin = D8;
const int inputPin = A0;
const int overflowIndicationPin = LED_BUILTIN;

int inputs[SENSOR_COUNT] = {0};
int startInput = 12;
int endInput = 15;

void setup() {
  Serial.begin(500000);
  pinMode(overflowIndicationPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW); // 4067 is active LOW
  for (int i = 0; i < 4; ++i) {
    pinMode(channelControlPins[i], OUTPUT);
  }
}

int maxx[SENSOR_COUNT];

void loop() {
  for(int t = 0; t < 2000; ++t) {
    collect();
    for (int i = 0; i < SENSOR_COUNT; ++i) {
      maxx[i] = max(maxx[i], inputs[i]);
    }
  }
  for (int i = 0; i < SENSOR_COUNT; ++i) {
    Serial.print(maxx[i]); Serial.print(' ');
    maxx[i] = 0;
  }
  Serial.println();
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

