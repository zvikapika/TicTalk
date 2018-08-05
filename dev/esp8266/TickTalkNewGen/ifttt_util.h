#include <ESP8266HTTPClient.h>

#define IFTTT_SERVER "maker.ifttt.com"

char ifttt_buff[150];

boolean ifttt_trigger(char* event, char* key, char* ing1, char* ing2, char* ing3) {
  boolean isSuccessful = false;
  String url = "http://" IFTTT_SERVER "/trigger/";
  url += event;
  url += "/with/key/";
  url += key;

  yield();
  boolean params = false;
  if (ing1 != NULL) {
    url += "?value1=";
    url += ing1;
    params = true;
  }
  yield();
  if (ing2 != NULL) {
    if (!params) {
      url += "?";
    }
    else {
      url += "&";
    }
    url += "value2=";
    url += ing2;
    params = true;
  }
  yield();
  if (ing3 != NULL) {
    if (!params) {
      url += "?";
    }
    else {
      url += "&";
    }
    yield();
    url += "value3=";
    url += ing3;
    params = true;
  }
  url.toCharArray(ifttt_buff, sizeof(ifttt_buff));
  Serial.print("connecting to ");
  Serial.println(ifttt_buff);

  HTTPClient http;
  
  http.begin(ifttt_buff); //HTTP
  
  int httpCode = http.GET();
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
      isSuccessful = true;
    } 
    else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
  }
  http.end();
  return isSuccessful;
}

boolean ifttt_trigger(char* event, char* key) {
  return ifttt_trigger(event, key, NULL, NULL, NULL);
}


