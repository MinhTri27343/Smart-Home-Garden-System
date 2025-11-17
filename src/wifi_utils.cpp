// #include "wifi_utils.h"
// #include <Arduino.h>

// void wifiConnect(const char* ssid, const char* password) {
//   Serial.print("Connecting to WiFi");
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print(".");
//     delay(500);
//   }
//   Serial.println(WiFi.localIP());
//   Serial.println(" Connected!");
// }

// void wifiEnsureConnected(const char* ssid, const char* password) {
//   if (WiFi.status() != WL_CONNECTED) {
//     Serial.println("WiFi lost! Reconnecting...");
//     wifiConnect(ssid, password);
//   }
// }
