#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

//***Set server***
const char* mqttServer = "172.16.16.63"; 
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
}

void mqttConnect() {
  while(!mqttClient.connected()) {
    Serial.println("Attemping MQTT connection...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    if(mqttClient.connect(clientId.c_str())) {
      Serial.println("connected ");
      //***Subscribe all topic you need***
      mqttClient.subscribe("home/temperature-sensor");
     
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
 
  //***Code here to process the received package***

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

    // Giả lập dữ liệu nhiệt độ, độ ẩm, hoặc trạng thái
    float t = dht.readTemperature(); // nếu có cảm biến thật thì đọc ở đây
    char msg[10];
    sprintf(msg, "%f", t);
    mqttClient.publish("home/temperature-sensor", msg);

    Serial.print("Published: ");
    Serial.println(msg);
  }
  
  delay(1000);
}