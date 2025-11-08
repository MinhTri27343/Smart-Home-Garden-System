#pragma once
#include <Arduino.h>

inline void printLine(const String& msg) {
  Serial.println("-----------------------------");
  Serial.println(msg);
  Serial.println("-----------------------------");
}

