#ifndef MYFILESYSTEM_H
#define MYFILESYSTEM_H

#include <Arduino.h>
#include "FS.h"
#include "LittleFS.h"
#include <ArduinoJson.h>

class MyFileSystem
{
public:
    // Initialize LittleFS filesystem
    static bool initialize();

    // Get filesystem information
    static void printInfo();

    // Create test file
    static bool createTestFile();

    // Read and display test file
    static bool readTestFile();

    // List all files in filesystem
    static void listFiles();

    // Print filesystem structure as tree
    static void printTree(const char *dirname = "/", uint8_t levels = 3);

    // Get filesystem status
    static size_t getTotalBytes();
    static size_t getUsedBytes();
    static size_t getFreeBytes();

    // JSON configuration functions
    static bool loadConfig();
    static bool saveConfig();
    static void printConfig();
    static String getConfigValue(const char *key);
    static bool setConfigValue(const char *key, const char *value);

    // WiFi configuration helpers
    static String getWiFiSSID();
    static String getWiFiPassword();
    static int getWiFiTimeout();
    static bool setWiFiCredentials(const char* ssid, const char* password);

private:
    static void printTreeRecursive(fs::FS &fs, const char *dirname, uint8_t levels, uint8_t indent);

    // JSON document for configuration
    static JsonDocument configDoc;
};

#endif // MYFILESYSTEM_H
