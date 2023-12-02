#include <WebServer.h>
#include <WiFi.h>
#include "esp_wpa2.h"

#include <ArduinoJson.h>

#include "config.h"

// WiFi code
WebServer server(80);

// project variables
int POTPIN = A0;

int currA0Val = 0;
int prevA0Val = 0;
int deltA0Val = 0;

int LEDPIN = 4;
long turnLedOffTime = 0;

// WiFi function
void handleNotFound() {
  server.send(404, "text/plain", "404! ADDRESS NOT FOUND");
}

void handleData() {
  StaticJsonDocument<128> resJson;
  JsonObject data = resJson.createNestedObject("data");
  JsonObject jA0 = data.createNestedObject("A0");

  jA0["value"] = currA0Val;
  jA0["delta"] = deltA0Val;

  String resTxt = "";
  serializeJson(resJson, resTxt);

  server.send(200, "application/json", resTxt);
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
  server.on("/data", handleData);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  // read input and calculate delta
  currA0Val = analogRead(POTPIN);

  deltA0Val = 0;
  if (currA0Val - prevA0Val > 5) {
    deltA0Val = 1;
  } else if (currA0Val - prevA0Val < -5) {
    deltA0Val = -1;
  }

  prevA0Val = currA0Val;

  // WiFi code
  server.handleClient();
  delay(2);
}
