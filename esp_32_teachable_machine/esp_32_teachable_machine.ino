#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"
const char* ssid = "DarpanN";
const char* password = "12345670";

const char* tm_server = "https://teachablemachine.withgoogle.com/models/13CmfUxEQ/"; // Replace this with your Teachable Machine server address

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Camera initialization
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
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.fb_count = 1;

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void loop() {
  // Capture image
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    return;
  }

  // Convert image to base64 format
  size_t fb_len = 0;
  uint8_t *fb_buf = fb->buf;
  size_t fb_size = fb->len;
  char *b64 = base64_encode(fb_buf, fb_size, &fb_len);

  // Send image to Teachable Machine
  HTTPClient http;
  http.begin(tm_server);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST("{\"image\": \"" + String(b64) + "\"}");
  String payload = http.getString();
  Serial.println(payload);

  // Extract class name from Teachable Machine response
  JSONVar response = JSON.parse(payload);
  String className = response["class"];
  Serial.println("Class: " + className);

  // Free memory
  free(b64);
  esp_camera_fb_return(fb);
  http.end();

  delay(5000); // Delay before capturing and sending the next image
}
