#include "esp_camera.h"
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

void startCameraServer();

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());

  // Initialize the camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Adjust camera settings
  sensor_t *sensor = esp_camera_sensor_get();

  // Set brightness
  sensor->set_brightness(sensor, 1);  // Range: -2 to 2

  // Set contrast
  sensor->set_contrast(sensor, 1);    // Range: -2 to 2

  // Set saturation
  sensor->set_saturation(sensor, 0);  // Range: -2 to 2

  // Set special effects
  sensor->set_special_effect(sensor, 0);  // Range: 0-7

  // Set white balance
  sensor->set_whitebal(sensor, true);    // Enable/Disable white balance

  // Set auto gain
  sensor->set_gain_ctrl(sensor, true);   // Enable/Disable auto gain

  // Set auto exposure
  sensor->set_exposure_ctrl(sensor, true);  // Enable/Disable auto exposure

  // Start streaming web server
  startCameraServer();
}

void loop() {
  delay(1);
}
