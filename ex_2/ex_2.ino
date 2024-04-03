#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "esp_camera.h"

const char* ssid = "DarpanN";
const char* password = "12345670";

const String teachable_machine_url = "https://teachablemachine.withgoogle.com/models/CcUd5ll0g/model.json";

void setup() {
  Serial.begin(115200);
  delay(10);

  // Initialize the camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  // Capture an image
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Send the image to the Teachable Machine model
  HTTPClient http;
  http.begin(teachable_machine_url);
  http.addHeader("Content-Type", "image/jpeg");
  int httpResponseCode = http.sendRequest("POST", fb->buf, fb->len);

  if (httpResponseCode == HTTP_CODE_FOUND) {
    // Handle the 302 Found response
    String redirectUrl = http.getString();
    Serial.println("Redirect URL: " + redirectUrl);

    // Send the image to the new URL
    http.begin(redirectUrl);
    http.addHeader("Content-Type", "image/jpeg");
    httpResponseCode = http.sendRequest("POST", fb->buf, fb->len);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(response);

      // Parse the response to get the class
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, response);
      const char* className = doc["className"];
      Serial.printf("Predicted class: %s\n", className);
    } else {
      Serial.printf("HTTP request failed, error: %d\n", httpResponseCode);
    }
  } else if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(response);

    // Parse the response to get the class
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, response);
    const char* className = doc["className"];
    Serial.printf("Predicted class: %s\n", className);
  } else {
    Serial.printf("HTTP request failed, error: %d\n", httpResponseCode);
  }

  http.end();
  esp_camera_fb_return(fb);
  delay(2000);
}