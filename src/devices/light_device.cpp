#include "light_device.h"

static int lightPin;

void lightDeviceInit(int pin) {
  lightPin = pin;
  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, LOW);
}

void lightDeviceCommand(const String& cmd) {
  if (cmd == "on") digitalWrite(lightPin, HIGH);
  else if (cmd == "off") digitalWrite(lightPin, LOW);
  Serial.print("[Light] Command: ");
  Serial.println(cmd);
}
