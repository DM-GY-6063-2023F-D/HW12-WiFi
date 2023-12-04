#include <WebServer.h>
#include <WiFi.h>

#include <ArduinoJson.h>

#include "config.h"

// WiFi code
WebServer server(80);

// project variables
int POTPIN = A0;

int currA0Val = 0;
int prevA0Val = 0;
float avgA0Val = 0;

int LEDPIN = 4;
long turnLedOffTime = 0;
int LED_DURATION = 2000;

// WiFi functions
void handleData() {
  StaticJsonDocument<128> resJson;
  JsonObject data = resJson.createNestedObject("data");
  JsonObject jA0 = data.createNestedObject("A0");

  jA0["raw"] = currA0Val;
  jA0["previous"] = prevA0Val;
  jA0["average"] = int(avgA0Val);

  String resTxt = "";
  serializeJson(resJson, resTxt);

  server.send(200, "application/json", resTxt);
}

// WebServer "documentation":
// https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer/src/WebServer.h
void handlePost() {
  StaticJsonDocument<128> postData;
  String postJson(server.arg(0));

  server.send(200, "text/plain", "POST");

  deserializeJson(postData, postJson);

  int mSuccess = postData["success"];

  if (mSuccess) {
    digitalWrite(LEDPIN, HIGH);
    turnLedOffTime = millis() + LED_DURATION;
  }
}

// Handle CORS pre-flight on POST requests, according to this:
// https://stackoverflow.com/a/74116471
void handleNotFound() {
  if (server.method() == HTTP_OPTIONS) {
    server.send(200);
  } else {
    server.send(404, "text/plain", "404! ADDRESS NOT FOUND");
  }
}

void setup() {
  // project setup
  Serial.begin(9600);
  while (!Serial) {}

  pinMode(LEDPIN, OUTPUT);

  // WiFi setup
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, SSID_PASSWORD);

  int tryConnectCount = 0;
  while (WiFi.status() != WL_CONNECTED && tryConnectCount < 60) {
    delay(500);
    Serial.print(".");
    tryConnectCount++;
  }
  Serial.println("");

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi is NOT connected! 💩");
  }

  server.enableCORS();
  server.on("/data", HTTP_GET, handleData);
  server.on("/post", HTTP_POST, handlePost);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  // keep previous value
  prevA0Val = currA0Val;

  // read input
  currA0Val = analogRead(POTPIN);

  // compute low-pass average
  avgA0Val = 0.99 * avgA0Val + 0.01 * currA0Val;

  // check if LED has to be turned off
  if (millis() > turnLedOffTime) {
    digitalWrite(LEDPIN, LOW);
    turnLedOffTime = millis() + 10 * LED_DURATION;
  }

  // WiFi code
  server.handleClient();
  delay(2);
}
