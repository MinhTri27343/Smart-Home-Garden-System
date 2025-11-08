#include <Arduino.h>
#include "config.h"
#include "wifi_utils.h"
#include "mqtt_utils.h"
#include "devices/soil_sensor.h"
#include "devices/light_device.h"

unsigned long lastRead = 0;

void setup()
{
  Serial.begin(9600);
  wifiConnect(WIFI_SSID, WIFI_PASSWORD);

  mqttSetup(MQTT_SERVER, MQTT_PORT);
  mqttReconnect();

  // Subscribe to control topics
  mqttSubscribe("home/led");
  topicHandlers["home/led"] = [](const String &msg)
  {
    lightDeviceCommand(msg);
  };

  // Initialize devices
  soilSensorInit(34);
  lightDeviceInit(2);
}

void loop()
{
  wifiEnsureConnected(WIFI_SSID, WIFI_PASSWORD);
  mqttReconnect();
  mqttLoop();

  // Run device logic
  if (millis() - lastRead >= intervalMs)
  {
    lastRead = millis();
    handlePublishSoilSensor("home/sensor");
  }
}
