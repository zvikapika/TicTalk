#include <time.h> 

int timezone = 3;
int dst = 0;

struct tm * ntptimeGet() {
  time_t now = time(nullptr);
  struct tm * timeinfo = localtime(&now);  
  return timeinfo;
}

String getTimeString(struct tm * timeinfo) {
  return String(timeinfo->tm_hour) + ':' + timeinfo->tm_min + ':' + timeinfo->tm_sec + ' ' + timeinfo->tm_mday + '/' + timeinfo->tm_mon + '/' + (1900 + timeinfo->tm_year);
}

void ntptimeSetup() {
  Serial.print("setting up ntp");
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
  Serial.println("\nwaiting for time update...");

  struct tm * timeinfo = ntptimeGet();
  while (timeinfo->tm_year == 70) { // if you think it's 1970, you need to get a fresh grip on reality
    delay(100);
    Serial.print(".");
    neopixelCircleUpdate(strip.Color(255, 255, 0));
//    now = time(nullptr);
//    timeinfo = localtime(&now);  
    timeinfo = ntptimeGet();
    // Serial.println(ctime(&now));
  }
  Serial.println();
  Serial.println(getTimeString(timeinfo));
}


int getMinuteDiff(int laterMinute, int earlierMinute) {
  if(laterMinute >= earlierMinute) {
    return laterMinute - earlierMinute;
  }
  else {
    return laterMinute + (60 - earlierMinute);
  }
}

