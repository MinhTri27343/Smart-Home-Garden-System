#pragma once
#include <Arduino.h>

void soilSensorInit(int pin);
void handlePublishSoilSensor(const String& baseTopic);

    