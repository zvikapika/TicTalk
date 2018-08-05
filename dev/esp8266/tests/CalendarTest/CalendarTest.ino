#define BLYNK_PRINT Serial

#include <Adafruit_NeoPixel.h>
#include <Regexp.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFiMulti.h>

#include "neopixel.h"

#define MAX_BUFFER_LEN 100

ESP8266WiFiMulti wifiMulti;


//char ssid[] = "ToiWork_2G";
//char pass[] = "OpenSes!";

char auth[] = "01dee24a40f747d4ad8b528cc82b224f";

char buff[MAX_BUFFER_LEN];

MatchState ms;

// http://188.166.206.43/01dee24a40f747d4ad8b528cc82b224f/update/V0?value=
// String sent: May 30, 2018 at 02:33PM-May 30, 2018 at 03:18PM

// http://188.166.206.43/01dee24a40f747d4ad8b528cc82b224f/update/V0?value=May 30, 2018 at 02:33PM-May 30, 2018 at 03:18PM

void setup() {
  Serial.begin(115200);
  delay(100);

  neopixelInit();
    
  wifiMulti.addAP("StudioForReal", "baklawa666666");
  wifiMulti.addAP("zvikoid", "baklawa666666");
  wifiMulti.addAP("GurEl14", "0523377036");
  wifiMulti.addAP("ToiWork_2G", "OpenSes!");

  int i = 0;
  Serial.println("connecting to wifi");
  while(wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    strip.setPixelColor(i, strip.Color(255, 0, 255));
    strip.show();
    delay(100);
    strip.setPixelColor(i, 0);
    strip.show();
    i = (i + 1) % strip.numPixels();
  }
  Serial.println("\nconnected!");
  rainbowCycle(1);
  colorWipe(0, 15);

  Blynk.config(auth);
  // Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
}

// Assuming String received in the format of:
// "May 30, 2018 at 02:33PM-May 30, 2018 at 03:18PM"
// (defined in IFTTT applet)
BLYNK_WRITE(V0) {
  String data = param.asString();
  Serial.println(String("got: ") + data);
  data.toCharArray(buff, constrain(data.length(), 0, MAX_BUFFER_LEN));
  ms.Target(buff);
  int result = ms.Match(" at ");
  if (result > 0) {
    int startHour = data.substring(ms.MatchStart + 4, ms.MatchStart + 6).toInt();
    int startMinute = data.substring(ms.MatchStart + 7, ms.MatchStart + 9).toInt();
    if (data.charAt(ms.MatchStart + 9) == 'P') {
      startHour += 12;
    }
    Serial.println(String("startHour=") + startHour);
    Serial.println(String("startMinute=") + startMinute);

    data = data.substring(ms.MatchStart + 10);
    data.toCharArray(buff, constrain(data.length(), 0, MAX_BUFFER_LEN));
    ms.Target(buff);
    result = ms.Match(" at ");
    if (result > 0) {
      int endHour = data.substring(ms.MatchStart + 4, ms.MatchStart + 6).toInt();
      int endMinute = data.substring(ms.MatchStart + 7, ms.MatchStart + 9).toInt();
      if (data.charAt(ms.MatchStart + 9) == 'P') {
        endHour += 12;
      }
      Serial.println(String("endHour=") + endHour);
      Serial.println(String("endMinute=") + endMinute);
      int duration = (endHour * 60 + endMinute - (startHour * 60 + startMinute));
      if(duration > 0) {
        onAppointment(duration);
      }
    }
    else {
      Serial.println("no match found for ending hour");
    }
  }
  else {
    Serial.println("no match found for starting hour");
  }
}


void onAppointment(int minuteCount) {
  Serial.println(String("Starting appointment, duration: ") + minuteCount + " minutes");
  unsigned long startt = millis();
  unsigned long endd = startt + minuteCount * 60 * 1000;
  int count = 0;
  while(millis() < endd) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((count++)%255));
      strip.show();
      delay(100);
      strip.setPixelColor(i, 0);
      strip.show();
      Blynk.run();
    }
  }
  Serial.println("Appointment ended!");
}


