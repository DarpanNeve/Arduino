#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "esp_camera.h"

// Replace with your network credentials
const char* ssid = "DarpanN";
const char* password = "12345670";

const String tm_endpoint = "https://teachablemachine.withgoogle.com/models/CcUd5ll0g/";

// Camera pin definitions
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

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
  config.pixel_format = PIXFORMAT_JPEG;

  // Initialize camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void loop() {
  // Capture image
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Encode image to Base64
  String base64Image = base64Encode(fb->buf, fb->len);
  Serial.println("Image encoded to Base64");

  // Send image to Teachable Machine
  String response = sendImageToTeachableMachine(base64Image);
  Serial.println("Response from Teachable Machine:");
  Serial.println(response);

  // Parse response (assuming JSON response)
  // Example parsing using ArduinoJson library
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, response);
  String predictedClass = doc["predictions"][0]["className"];
  Serial.print("Predicted class: ");
  Serial.println(predictedClass);

  // Free the camera frame buffer
  esp_camera_fb_return(fb);

  delay(5000);  // delay before capturing next image
}

String base64Encode(const uint8_t* data, size_t len) {
  const char* charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String encoded = "";
  for (size_t i = 0; i < len; i += 3) {
    uint32_t temp = 0;
    temp |= data[i];
    if (i + 1 < len) {
      temp <<= 8;
      temp |= data[i + 1];
    }
    if (i + 2 < len) {
      temp <<= 8;
      temp |= data[i + 2];
    }

    for (int j = 0; j < 4; j++) {
      if (i * 8 + j * 6 > 8 * len) {
        encoded += '=';
      } else {
        encoded += charSet[(temp >> (6 * (3 - j))) & 0x3F];
      }
    }
  }
  return encoded;
}

String sendImageToTeachableMachine(String base64Image) {
  WiFiClientSecure client;
  if (!client.connect(tm_endpoint.c_str(), 443)) {
    Serial.println("Connection to Teachable Machine failed");
    return "";
  }

  String requestBody = "{\"instances\": [{\"image_bytes\": {\"b64\": \"" + base64Image + "\"}}]}";
  client.print(String("POST ") + tm_endpoint + " HTTP/1.1\r\n" + "Host: " + tm_endpoint + "\r\n" + "Content-Type: application/json\r\n" + "Content-Length: " + requestBody.length() + "\r\n" + "Connection: close\r\n\r\n" + requestBody + "\r\n");

  String response = "";
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  while (client.available()) {
    response += client.readStringUntil('\n');
  }
  client.stop();
  return response;
}
