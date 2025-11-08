#include "soil_sensor.h"
#include "../mqtt_utils.h"
#include <Arduino.h>

static int soilPin;
static unsigned long lastRead = 0;

void soilSensorInit(int pin) {
  soilPin = pin;
  pinMode(soilPin, INPUT);
}

void handlePublishSoilSensor(const String& topic) {
    int value = analogRead(soilPin);
    mqttPublish(topic, String(value));
    Serial.print("[Soil] Published ");
    Serial.print(value);
    Serial.print(" to ");
    Serial.println(topic);
}
