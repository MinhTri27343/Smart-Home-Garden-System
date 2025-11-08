#ifndef MQTT_UTILS_H
#define MQTT_UTILS_H
#include <PubSubClient.h>
#include <WiFi.h>
#include <map>
// Type for topic handler
using MqttHandler = std::function<void(const String&)>;

void mqttSetup(const char* server, int port);
void mqttLoop();
void mqttReconnect();
void mqttPublish(const String& topic, const String& payload);
void mqttSubscribe(const String& topic);

// Topic map
extern std::map<String, MqttHandler> topicHandlers;

#endif
