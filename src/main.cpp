#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

// ==========================================
// 1. CẤU HÌNH
// ==========================================

// --- Khai báo chân (Pin Definitions) ---
#define PIN_LIGHT       32
#define PIN_DHT         33
#define PIN_WATER       34
#define PIN_HUMIDITY    35
#define PIN_RELAY       18

// --- Loại cảm biến ---
#define DHT_TYPE        DHT11

// --- Thông tin WiFi ---
const char *SSID_NAME = "Minh Tri";
const char *SSID_PASS = "26072005";

// --- Thông tin MQTT ---
const char *MQTT_SERVER = "192.168.1.43"; // IP của Broker
const int   MQTT_PORT   = 1883;


// --- Các Topic  ---
// Topic nhận lện
const char *TOPIC_CONTROL = "device/water"; 

// Topic gửi dữ liệu 
const char *TOPIC_SENSOR_WATER = "sensor/water";
const char *TOPIC_SENSOR_LIGHT = "sensor/light";
const char *TOPIC_SENSOR_TEMP  = "sensor/temperature";
const char *TOPIC_SENSOR_HUMID = "sensor/humid";

// ==========================================
// 2. KHỞI TẠO BIẾN & ĐỐI TƯỢNG
// ==========================================

// --- Khởi tạo biến ---
DHT dht(PIN_DHT, DHT_TYPE);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
WiFiManager wifiManager; // Khởi tạo WiFiManager

// Biến quản lý hẹn giờ
unsigned long pumpOffTime  = 0;
bool isPumpTimerActive  = false;

// Biến quản lý thời gian gửi tin nhắn
unsigned long lastSendTime = 0;
const long INTERVAL_SEND = 2000; // Gửi dữ liệu mỗi 2000ms (2 giây)

// ==========================================
// 3. CÁC HÀM XỬ LÝ KẾT NỐI
// ==========================================

// void setupWifi() {
//   delay(10);
//   Serial.println();
//   Serial.print("Dang ket noi WiFi: ");
//   Serial.println(SSID_NAME);

//   WiFi.begin(SSID_NAME, SSID_PASS);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("\nWiFi da ket noi!");
//   Serial.print("IP Address: ");
//   Serial.println(WiFi.localIP());
// }

void reconnectMqtt() {
  // Lặp cho đến khi kết nối được
  while (!mqttClient.connected()) {
    Serial.print("Dang ket noi MQTT...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Thanh cong!");
      // Đăng ký nhận tin từ topic điều khiển
      mqttClient.subscribe(TOPIC_CONTROL);
    } else {
      Serial.print("That bai, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" -> Thu lai sau 5s");
      delay(5000);
    }
  }
}

// ==========================================
// 4. HÀM XỬ LÝ LOGIC (CALLBACK & SENSOR)
// ==========================================

// Hàm xử lý khi nhận tin nhắn MQTT
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Nhan tin nhan tu [");
  Serial.print(topic);
  Serial.print("]: ");

  // 1. Parse JSON
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print("Loi doc JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // 2. Lấy dữ liệu và chuẩn hóa
  String lenh = String((const char*)doc["turn"]); 
  int soGiay  = doc["second"];
  
  lenh.toUpperCase(); // Chuyển thành "ON", "OFF" (chữ hoa) để so sánh chuẩn xác


  // 3. Xử lý Logic
  if (lenh == "OFF") {
    digitalWrite(PIN_RELAY, LOW); 
    isPumpTimerActive  = false;          // Hủy hẹn giờ
  } 
  else if (lenh == "ON") {
    digitalWrite(PIN_RELAY, HIGH); // Bật bơm
    
    if (soGiay > 0) {
      // Thiết lập hẹn giờ tắt
      pumpOffTime  = millis() + (soGiay * 1000UL);
      isPumpTimerActive  = true;
    } else {
      // Bật mãi mãi
      isPumpTimerActive  = false;
    }
  }
}

// Hàm đọc cảm biến và gửi dữ liệu
void readAndPublishSensors() {
  // 1. Water Sensor
  int waterValue = analogRead(PIN_WATER);
  mqttClient.publish(TOPIC_SENSOR_WATER, String(waterValue).c_str());
  Serial.printf("Water: %d\n", waterValue);

  // 2. Light Sensor
  int lightValue = analogRead(PIN_LIGHT);
  mqttClient.publish(TOPIC_SENSOR_LIGHT, String(lightValue).c_str());
  Serial.printf("Light: %d\n", lightValue);

  // 3. Humidity (Soil) Sensor
  int humidValue = analogRead(PIN_HUMIDITY);
  mqttClient.publish(TOPIC_SENSOR_HUMID, String(humidValue).c_str());
  Serial.printf("Humid: %d\n", humidValue);

  // 4. DHT Sensor (Temperature)
  float tempValue = dht.readTemperature();
  
  
  Serial.println("-----------------------");
}

// ==========================================
// 5. MAIN SETUP & LOOP
// ==========================================

void setup() {
  Serial.begin(9600);
  
  // Cấu hình chân
  pinMode(PIN_WATER, INPUT);
  pinMode(PIN_LIGHT, INPUT);
  pinMode(PIN_HUMIDITY, INPUT);
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW); // Mặc định tắt bơm

  // Khởi động cảm biến
  dht.begin();

  // Khởi động wifi

  wifiManager.resetSettings();

  wifiManager.autoConnect("CaMoi3CoGai");

  // Kết nối mạng
  // setupWifi();

  Serial.println("\nWiFi da ket noi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("connect success , ye");

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(callback);
  mqttClient.setKeepAlive(90); 

  Serial.println("connect success , ye2");
}

void loop() {
  // 1. Kiểm tra kết nối WiFi
  if (WiFi.status() != WL_CONNECTED) {
    // setupWifi();
    Serial.println("Mat ket noi WiFi...");
  }

  // 2. Kiểm tra kết nối MQTT
  if (!mqttClient.connected()) {
    reconnectMqtt();
  }
  
  // 3. Duy trì MQTT
  mqttClient.loop();

  // 4. Logic tự động tắt bơm (Non-blocking)
  if (isPumpTimerActive  && millis() >= pumpOffTime ) {
    digitalWrite(PIN_RELAY, LOW); // Tắt bơm
    isPumpTimerActive  = false;           // Reset cờ
  }

  // 5. Đọc và gửi cảm biến định kỳ (Non-blocking)
  if (millis() - lastSendTime > INTERVAL_SEND) {
    lastSendTime = millis();
    readAndPublishSensors();
  }
}