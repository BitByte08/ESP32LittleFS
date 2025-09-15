#include "myFileSystem.h"

bool initLittleFS(){
    if(!LittleFS.begin(true)){
        Serial.println("FS Mount Error!");
        return false;
    }
    Serial.println("FS Mount Succes!");
    return true;
}

bool writeLittleFS() {
    File f = LittleFS.open("/myWiFi.txt", FILE_WRITE);
    f.println("SSID");
    f.close();
}

bool readLittleFS() {
    File f = LittleFS.open("/myWiFi.txt", FILE_READ);
    while(f.available()) Serial.write(f.read());
    f.close();
}

void printFilesystemInfo()
{
  size_t totalBytes = LittleFS.totalBytes();
  size_t usedBytes = LittleFS.usedBytes();
  Serial.print("[LittleFS] total=");
  Serial.print(totalBytes);
  Serial.print(" bytes, used=");
  Serial.print(usedBytes);
  Serial.println(" bytes");
}

void printTree(fs::FS &fs, const char *dirname, uint8_t levels, uint8_t indent)
{
  File root = fs.open(dirname);
  if (!root || !root.isDirectory())
  {
    Serial.println("[LittleFS] tree print failed: invalid path");
    return;
  }

  File entry = root.openNextFile();
  while (entry)
  {
    for (uint8_t i = 0; i < indent; i++)
      Serial.print("  ");
    if (entry.isDirectory())
    {
      Serial.print("|- ");
      Serial.print(entry.name());
      Serial.println("/");
      if (levels)
      {
        printTree(fs, entry.name(), levels - 1, indent + 1);
      }
    }
    else
    {
      Serial.print("|- ");
      Serial.print(entry.name());
      Serial.print("  (");
      Serial.print(entry.size());
      Serial.println(" bytes)");
    }
    entry = root.openNextFile();
  }
}