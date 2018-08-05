#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;

void wifiConnect() {
  WiFi.mode(WIFI_STA);

  wifiMulti.addAP("StudioForReal", "baklawa666666");
  wifiMulti.addAP("zvikoid", "baklawa666666");
  wifiMulti.addAP("GurEl14", "0523377036");
  wifiMulti.addAP("ToiWork_2G", "OpenSes!");

  Serial.println("\nConnecting to WiFi");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    neopixelCircleUpdate(strip.Color(255, 0, 255));
    delay(100);
  }
  Serial.println("\nconnected!");
  WiFi.printDiag(Serial);
}

void blynk_setup() {
  
}

