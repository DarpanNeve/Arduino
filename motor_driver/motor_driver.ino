int motor1pin1 = 2;
int motor1pin2 = 3;
int motor2pin1 = 4;
int motor2pin2 = 5;
int val = 0;
String input="0";
void setup() {
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, HIGH);
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, HIGH);
  delay(1000);
  Serial.begin(115200);
  Serial.println("Setup complete");
}

void loop() {
  if (Serial.available() > 0) {
    input = Serial.readStringUntil('\n');  // Read the input until newline character                        // Convert the string to an integer
    Serial.print("Received value: ");
    Serial.println(input);
  }
  if (input == "1" || input == 1) {
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
    Serial.println("Turning motors for Class3");
  } else if (input == "2" || input == 2) {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
    Serial.println("Stopping motors for Stop-sign");
  } else if (input == "3" || input == 3) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    Serial.println("Turning motors for U-turn");
  }
}
