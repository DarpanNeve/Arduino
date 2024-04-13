void setup() {
    Serial.begin(9600); // Initialize serial communication
}

void loop() {
    Serial.println("Hello from ESP32-CAM!");
    delay(1000); // Send a message every second
}
