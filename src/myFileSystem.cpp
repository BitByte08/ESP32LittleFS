#include "myFileSystem.h"

// Initialize static member
JsonDocument MyFileSystem::configDoc;

bool MyFileSystem::initialize()
{
    Serial.println("=== ESP32S3 LittleFS Test ===");

    // Initialize LittleFS (without auto-format)
    if (!LittleFS.begin(false))
    {
        Serial.println("LittleFS mount failed!");
        Serial.println("First time setup - formatting partition...");

        // Format only on first mount failure
        if (LittleFS.format())
        {
            Serial.println("Format completed!");
            if (LittleFS.begin(false))
            {
                Serial.println("LittleFS mount successful after format!");
                return true;
            }
            else
            {
                Serial.println("Mount failed even after format!");
                return false;
            }
        }
        else
        {
            Serial.println("Format failed!");
            return false;
        }
    }
    else
    {
        Serial.println("LittleFS mount successful!");
        return true;
    }
}

void MyFileSystem::printInfo()
{
    Serial.printf("Total size: %d bytes\n", getTotalBytes());
    Serial.printf("Available: %d bytes\n", getFreeBytes());
    Serial.printf("Used: %d bytes\n", getUsedBytes());
}

bool MyFileSystem::createTestFile()
{
    File file = LittleFS.open("/test.txt", "w");
    if (file)
    {
        file.println("Hello LittleFS!");
        file.println("ESP32-S3 filesystem test");
        file.printf("Current time: %lu ms\n", millis());
        file.close();
        Serial.println("test.txt file created successfully!");
        return true;
    }
    else
    {
        Serial.println("File creation failed!");
        return false;
    }
}

bool MyFileSystem::readTestFile()
{
    File file = LittleFS.open("/test.txt", "r");
    if (file)
    {
        Serial.println("=== File Content ===");
        while (file.available())
        {
            Serial.write(file.read());
        }
        file.close();
        Serial.println("=== File Read Complete ===");
        return true;
    }
    else
    {
        Serial.println("File read failed!");
        return false;
    }
}

void MyFileSystem::listFiles()
{
    Serial.println("=== File List ===");
    File root = LittleFS.open("/");
    File file_item = root.openNextFile();
    while (file_item)
    {
        Serial.printf("File: %s, Size: %d bytes\n",
                      file_item.name(), file_item.size());
        file_item = root.openNextFile();
    }
    root.close();
}

void MyFileSystem::printTree(const char *dirname, uint8_t levels)
{
    Serial.println("=== File System Tree ===");
    printTreeRecursive(LittleFS, dirname, levels, 0);
}

void MyFileSystem::printTreeRecursive(fs::FS &fs, const char *dirname, uint8_t levels, uint8_t indent)
{
    File root = fs.open(dirname);
    if (!root || !root.isDirectory())
    {
        Serial.println("[LittleFS] tree print failed: invalid path");
        if (root)
            root.close(); // Close even if invalid
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
                printTreeRecursive(fs, entry.name(), levels - 1, indent + 1);
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
        entry.close(); // Close current entry before getting next
        entry = root.openNextFile();
    }
    root.close(); // Close root directory
}

size_t MyFileSystem::getTotalBytes()
{
    return LittleFS.totalBytes();
}

size_t MyFileSystem::getUsedBytes()
{
    return LittleFS.usedBytes();
}

size_t MyFileSystem::getFreeBytes()
{
    return LittleFS.totalBytes() - LittleFS.usedBytes();
}

bool MyFileSystem::loadConfig()
{
    File file = LittleFS.open("/config.txt", "r");
    if (!file)
    {
        Serial.println("Failed to open config.txt");
        return false;
    }

    // Clear previous config
    configDoc.clear();

    // Parse JSON
    DeserializationError error = deserializeJson(configDoc, file);
    file.close();

    if (error)
    {
        Serial.print("Failed to parse config.json: ");
        Serial.println(error.c_str());
        return false;
    }

    Serial.println("Config loaded successfully!");
    return true;
}

bool MyFileSystem::saveConfig()
{
    File file = LittleFS.open("/config.txt", "w");
    if (!file)
    {
        Serial.println("Failed to open config.txt for writing");
        return false;
    }

    // Serialize JSON to file
    size_t bytesWritten = serializeJson(configDoc, file);
    file.close();

    if (bytesWritten == 0)
    {
        Serial.println("Failed to write config");
        return false;
    }

    Serial.println("Config saved successfully!");
    return true;
}

void MyFileSystem::printConfig()
{
    Serial.println("=== Configuration ===");
    serializeJsonPretty(configDoc, Serial);
    Serial.println();
}

String MyFileSystem::getConfigValue(const char* key)
{
    if (configDoc.containsKey(key))
    {
        return configDoc[key].as<String>();
    }
    return "";
}

bool MyFileSystem::setConfigValue(const char* key, const char* value)
{
    configDoc[key] = value;
    return saveConfig();
}

String MyFileSystem::getWiFiSSID()
{
    if (configDoc.containsKey("wifi") && configDoc["wifi"].containsKey("ssid"))
    {
        return configDoc["wifi"]["ssid"].as<String>();
    }
    return "";
}

String MyFileSystem::getWiFiPassword()
{
    if (configDoc.containsKey("wifi") && configDoc["wifi"].containsKey("password"))
    {
        return configDoc["wifi"]["password"].as<String>();
    }
    return "";
}

bool MyFileSystem::setWiFiCredentials(const char* ssid, const char* password)
{
    // wifi 객체가 없으면 생성
    if (!configDoc.containsKey("wifi")) {
        configDoc["wifi"] = JsonObject();
    }
    
    // WiFi 설정 업데이트
    configDoc["wifi"]["ssid"] = ssid;
    configDoc["wifi"]["password"] = password;
    configDoc["wifi"]["timeout"] = 10000;
    
    return saveConfig();
}

int MyFileSystem::getWiFiTimeout()
{
    if (configDoc.containsKey("wifi") && configDoc["wifi"].containsKey("timeout"))
    {
        return configDoc["wifi"]["timeout"].as<int>();
    }
    return 10000; // Default timeout
}
