#pragma once
#include <Arduino.h>

void lightDeviceInit(int pin);
void lightDeviceCommand(const String& cmd);

