#include <Arduino.h>
#include <WiFi.h>
#include "myFileSystem.h"

const char* ssid = "";
const char* password = "";

void setup(){
  Serial.begin(115200);
  initLittleFS();
  writeLittleFS();
  //readLittleFS();
  printTree(LittleFS, "/", 3, 0);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
}

void loop(){
  
}