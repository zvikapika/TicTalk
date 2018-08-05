#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <time.h>

ESP8266WiFiMulti wifiMulti;

const char* ssid = "zvikoid";
const char* password = "baklawa666666";

int timezone = 3;
int dst = 0;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("StudioForReal", "baklawa666666");
  wifiMulti.addAP("zvikoid", "baklawa666666");
  wifiMulti.addAP("GurEl14", "0523377036");
  wifiMulti.addAP("ToiWork_2G", "OpenSes!");

  Serial.println("\nConnecting to WiFi");
  while(wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  delay(1000);
  Serial.println("");
}

void loop() {
  time_t now = time(nullptr);
  Serial.println(ctime(&now));
  delay(1000);
}
