#include <Arduino.h>
#include "myFileSystem.h"

void setup(){
  Serial.begin(115200);
  initLittleFS();
  writeLittleFS();
  readLittleFS();
}

void loop(){
  
}