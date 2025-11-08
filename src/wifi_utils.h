#pragma once
#include <WiFi.h>

void wifiConnect(const char* ssid, const char* password);
void wifiEnsureConnected(const char* ssid, const char* password);

