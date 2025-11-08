// #include <WiFi.h>
// #include <FirebaseESP32.h>


// // Thông tin WiFi
// const char* ssid = "Wokwi-GUEST";
// const char* password = "";

// // Firebase Info
// #define DATABASE_URL "https://smart-home-garden-system-f599b-default-rtdb.firebaseio.com" 
// #define API_KEY "AIzaSyALc4YdhlS0YTy7JhLpXVUPoTYgxY0GO_g"

// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;

// int sensorPin = 34;  // Chân analog đọc độ ẩm đất
// int sensorValue = 0;

// void setup() {
//   Serial.begin(9600);

//   // Kết nối WiFi
//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//   Serial.print("Connecting to WiFi");
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print(".");
//     delay(300);
//   }
//   Serial.println("\n✅ WiFi connected!");

//   // Cấu hình Firebase
//   config.api_key = API_KEY;
//   config.database_url = DATABASE_URL;

//   // Không cần đăng nhập vì rule mở
//   Firebase.begin(&config, &auth);
//   Firebase.reconnectWiFi(true);
// }

// void loop() {
//   sensorValue = analogRead(sensorPin);
//   Serial.print("Soil Moisture: ");
//   Serial.println(sensorValue);

//   // Gửi dữ liệu lên Firebase
//   if (Firebase.RTDB.setInt(&fbdo, "/sensors/soil", sensorValue)) {
//     Serial.println("✅ Data sent to Firebase!");
//   } else {
//     Serial.print("❌ Error: ");
//     Serial.println(fbdo.errorReason());
//   }

//   delay(5000); // gửi mỗi 5 giây
// }
