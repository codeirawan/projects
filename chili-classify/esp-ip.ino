#include <WiFi.h>

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Wait until connected to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Print success info and IP address every 10 seconds
  static unsigned long lastPrintTime = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - lastPrintTime >= 10000) {  // 10 seconds
    lastPrintTime = currentMillis;

    Serial.println("Still connected to WiFi.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }

  // Add other code here as needed
}
