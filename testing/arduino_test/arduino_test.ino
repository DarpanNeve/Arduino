void setup() {
    Serial.begin(9600); // Initialize serial communication
}

void loop() {
    if (Serial.available()) {
        // If data is available to read
        char receivedChar = Serial.read(); // Read the incoming byte
        Serial.print("Received: ");
        Serial.println(receivedChar); // Print the received character
    }
}
