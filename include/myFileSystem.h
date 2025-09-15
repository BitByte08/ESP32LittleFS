#ifndef MYFILESYSTEM_H
#define MYFILESYSTEM_H

#include <Arduino.h>
#include <LittleFS.h>

bool initLittleFS();
bool writeLittleFS();
bool readLittleFS();
void printFilesystemInfo();
void printTree(fs::FS &fs, const char *dirname, uint8_t levels, uint8_t indent);

#endif