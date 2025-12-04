#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <WifiManager.h>

#define DHTPIN 27 
#define HUMIDPIN 34
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);


// const char* ssid = "Wokwi-GUEST";     //***Your WiFi SSID***
// const char* password = ""; //***Your WiFi Password***


const char* ssid = "iPhone";   
const char* password = "ngaysinhcuatao"; 

//***Set server***
const char* mqttServer = "172.20.10.3"; 
int port = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void wifiConnect() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
   Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void mqttConnect() {
  while(!mqttClient.connected()) {
    Serial.println("Attemping MQTT connection...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    if(mqttClient.connect(clientId.c_str())) {
      Serial.println("connected ");
      //***Subscribe all topic you need***
      mqttClient.subscribe("home/humid-sensor");
     
    }
    else {
      Serial.print(mqttClient.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

//MQTT Receiver
void callback(char* topic, byte* message, unsigned int length) {
  Serial.println(topic);
  String msg;
  for(int i=0; i<length; i++) {
    msg += (char)message[i];
  }
  Serial.println(msg);
}

void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to WiFi");
  dht.begin();


  wifiConnect();
  mqttClient.setServer(mqttServer, port);
  mqttClient.setCallback(callback);
  mqttClient.setKeepAlive( 90 );
}


void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Reconnecting to WiFi");
    wifiConnect();
  }
  if(!mqttClient.connected()) {
    mqttConnect();
  }
  mqttClient.loop();


 static unsigned long lastSend = 0;
  if (millis() - lastSend > 2000) {
    lastSend = millis();

    float t = dht.readTemperature();
    char msg2[10];
    sprintf(msg2, "%f", t);
    mqttClient.publish("home/temperature-sensor", msg2);
    Serial.print("Gia tri temperature: ");
    Serial.println(msg2);

  }
  
  delay(50);
}