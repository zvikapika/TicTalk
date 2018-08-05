#include <time.h>

int timezone = 3;
int dst = 0;

time_t ntptimeGet() {
  time_t now = time(nullptr);
  return now;
}

void ntptimeSetup() {
  configTime(timezone * 3600, dst, "pool.ntp.org", "time.nist.gov");
  while (!time(nullptr)) {
    Serial.print(".");
    strip.setPixelColor(runningPixelPosition, strip.Color(255, 255, 0));
    strip.show();
    delay(100);
    strip.setPixelColor(runningPixelPosition, 0);
    strip.show();
    runningPixelPosition = (runningPixelPosition + 1) % strip.numPixels();
  }
  Serial.println(ntptimeGet());
}




