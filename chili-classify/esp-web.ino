#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

WebServer server(80);  // Create a web server on port 80

void handleRoot() {
  server.send(200, "text/plain", "Hello, this is ESP32!");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);  // Handle requests to the root ("/") path

  server.begin();  // Start the web server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();  // Handle incoming client requests

  // Print success info and IP address every 10 seconds
  static unsigned long lastPrintTime = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - lastPrintTime >= 10000) {  // 10 seconds
    lastPrintTime = currentMillis;

    Serial.println("Still connected to WiFi.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
}
