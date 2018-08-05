#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_LED_COUNT 32
#define NEOPIXEL_PIN D3

int runningPixelPosition = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_LED_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void neopixelInit() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

uint32_t Wheel(byte WheelPos, int brightness = 255) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color((255 - WheelPos * 3) * brightness / 255, 0, (WheelPos * 3) * brightness / 255);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, (WheelPos * 3) * brightness / 255, (255 - WheelPos * 3) * brightness / 255);
  }
  WheelPos -= 170;
  return strip.Color((WheelPos * 3) * brightness / 255, (255 - WheelPos * 3) * brightness / 255, 0);
}

void rainbowCycle(uint8_t wait) {
  strip.setBrightness(255);
  uint16_t i, j;
  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

int circleIndex = 0;

void neopixelCircleUpdate(uint32_t c) {
  strip.setPixelColor(circleIndex , 0);
  circleIndex  = (circleIndex  + 1) % strip.numPixels();
  strip.setPixelColor(circleIndex , c);
  strip.show();
}

void neopixelUpdateMeetingInProgress() {
  int count = 0;
  //  while (millis() < endd) {
  //    for (uint16_t i = 0; i < strip.numPixels(); i++) {
  //      strip.setPixelColor(i, Wheel((count++) % 255));
  //      strip.show();
  //      delay(100);
  //      strip.setPixelColor(i, 0);
  //      strip.show();
  //      Blynk.run();
  //    }
  //  }
}

int chaser = 0;
//Theatre-style crawling lights.
void neopixelTheaterChase(uint32_t c, uint8_t wait) {
  for (int q = 0; q < 3; q++) {
    strip.show();
    for (int brightness = 0; brightness < 100; brightness += 1) {
      for (uint16_t i = q; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.setBrightness(brightness);
      strip.show();
      delay(1);
    }
    delay(wait);
    for (int brightness = 100; brightness >= 0; brightness -= 1) {
      for (uint16_t i = q; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.setBrightness(brightness);
      strip.show();
      delay(3);
    }
    for (uint16_t i = q; i < strip.numPixels(); i = i + 3) {
      strip.setBrightness(255);  //turn every third pixel on
      strip.setPixelColor(i + q, 0);  //turn every third pixel on
    }
    delay(wait);

    for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
      strip.setPixelColor(i + q, 0);      //turn every third pixel off
    }
  }
  Serial.println("out");
}


void neopixelFadeInOut(int pc, uint32_t c) {
  for (int brightness = 0; brightness < 100; brightness += 1) {
    for (uint16_t i = 0; i < pc; i = i + 1) {
      strip.setPixelColor(i, c);
    }
    strip.setBrightness(brightness);
    strip.show();
    delay(3);
  }
  delay(50);
  for (int brightness = 100; brightness >= 0; brightness -= 1) {
    for (uint16_t i = 0; i < pc; i = i + 1) {
      strip.setPixelColor(i, c);
    }
    strip.setBrightness(brightness);
    strip.show();
    delay(5);
  }
}

