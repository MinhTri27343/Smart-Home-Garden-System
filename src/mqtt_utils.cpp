// #include "mqtt_utils.h"
// #include <Arduino.h>

// WiFiClient wifiClient; // ✅ Define once here
// PubSubClient mqttClient(wifiClient);

// std::map<String, MqttHandler> topicHandlers;

// void mqttCallback(char *topic, byte *payload, unsigned int length)
// {
//   String lastTopic = String(topic);
//   String msg;
//   for (int i = 0; i < length; i++)
//   {
//     msg += (char)payload[i];
//   }
//   Serial.println(msg);

//   // Example: handle light command directly
//   if (topicHandlers.find(lastTopic) != topicHandlers.end())
//   {
//     topicHandlers[lastTopic](msg); // call handler
//   }
//   else
//   {
//     Serial.println("No handler for topic: " + lastTopic);
//   }
// }

// void mqttSetup(const char *server, int port)
// {
//   mqttClient.setServer(server, port);
//   mqttClient.setCallback(mqttCallback);
// }

// void mqttLoop()
// {
//   mqttClient.loop();
// }

// void mqttReconnect()
// {
//   while (!mqttClient.connected())
//   {
//     Serial.print("Attempting MQTT connection...");
//     String clientId = "ESP32-" + String(random(0xffff), HEX);
//     if (mqttClient.connect(clientId.c_str()))
//     {
//       Serial.println("Connected!");
//     }
//     else
//     {
//       Serial.print("Failed, rc=");
//       Serial.println(mqttClient.state());
//       delay(5000);
//     }
//   }
// }

// void mqttPublish(const String &topic, const String &payload)
// {
//   mqttClient.publish(topic.c_str(), payload.c_str());
// }

// void mqttSubscribe(const String &topic)
// {
//   mqttClient.subscribe(topic.c_str());
// }
