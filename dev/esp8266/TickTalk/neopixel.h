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
  if(WheelPos < 85) {
    return strip.Color((255 - WheelPos * 3) * brightness / 255, 0, (WheelPos * 3) * brightness / 255);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, (WheelPos * 3) * brightness / 255, (255 - WheelPos * 3) * brightness / 255);
  }
  WheelPos -= 170;
  return strip.Color((WheelPos * 3) * brightness / 255, (255 - WheelPos * 3) * brightness / 255, 0);
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

