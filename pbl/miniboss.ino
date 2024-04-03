#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Function prototypes
float readTemperature();
float readpH();
void rotateServo();

float calibration_value = 21.34 - 0.7 + (7.0 - 4.0);
int phval = 0;
unsigned long int avgval; 
int buffer_arr[10],temp;

float ph_act;

// Servo motor setup
Servo servo;
const int SERVO_PIN = 9;

// Temperature sensor setup
#define ONE_WIRE_BUS 10
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Relay modules setup
const int RELAY1_PIN = 4;  // Connected to heating coil relay
const int RELAY2_PIN = 5;  // Connected to pH-related relay

// Time variables for servo rotation
const unsigned long SERVO_PERIOD = 30 * 1000;  // 30 seconds in milliseconds
unsigned long lastServoTime = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize servo motor
  servo.attach(SERVO_PIN);

  // Initialize relay pins
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);

  // Start temperature sensor
  sensors.begin();
}

void loop() {
  // Check if there is data available on the serial port
  if (Serial.available()) {
    // Read the incoming data
    String data = Serial.readStringUntil('\n');
    // Process the received data
    Serial.println(data);

    // Check the received data
    if (data == "B") {
      // Temperature and pH ranges
      const float TEMP_MIN = 25.0;  // Minimum temperature threshold
      const float TEMP_MAX = 30.0;  // Maximum temperature threshold
      const float PH_MIN = 6.0;     // Minimum pH threshold
      const float PH_MAX = 8.0;     // Maximum pH threshold

      // Read temperature
      float temperature = readTemperature();

      // Read pH
      float pH = readpH();

      // Print temperature and pH values
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" °C");

      Serial.print("pH Value: ");
      Serial.println(pH);

      // Check temperature range
      if (temperature < TEMP_MIN) {
        // Turn on relay1 (heating coil)
        digitalWrite(RELAY1_PIN, HIGH);
      } else {
        // Turn off relay1 (heating coil)
        digitalWrite(RELAY1_PIN, LOW);
      }

      // Check pH range
      if (pH < PH_MIN || pH > PH_MAX) {
        // Turn on relay2 (pH-related action)
        digitalWrite(RELAY2_PIN, HIGH);
      } else {
        // Turn off relay2 (pH-related action)
        digitalWrite(RELAY2_PIN, LOW);
      }
      Serial.print("You have a Black and White Fish! \n");
    }
      if (data == "P") {
        // Temperature and pH ranges
        const float TEMP_MIN = 25.0;  // Minimum temperature threshold
        const float TEMP_MAX = 30.0;  // Maximum temperature threshold
        const float PH_MIN = 6.0;     // Minimum pH threshold
        const float PH_MAX = 8.0;     // Maximum pH threshold

        // Read temperature
        float temperature = readTemperature();

        // Read pH
        float pH = readpH();

        // Print temperature and pH values
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" °C");

        Serial.print("pH Value: ");
        Serial.println(pH);
        rotateServo();

        // Check temperature range
        if (temperature < TEMP_MIN) {
          // Turn on relay1 (heating coil)
          digitalWrite(RELAY1_PIN, HIGH);
        } else {
          // Turn off relay1 (heating coil)
          digitalWrite(RELAY1_PIN, LOW);
        }

        // Check pH range
        if (pH < PH_MIN || pH > PH_MAX) {
          // Turn on relay2 (pH-related action)
          digitalWrite(RELAY2_PIN, HIGH);
        } else {
          // Turn off relay2 (pH-related action)
          digitalWrite(RELAY2_PIN, LOW);
        }
        Serial.print("You have a Pink Fish! \n");
      }
        if (data == "I") {
        // Temperature and pH ranges
        const float TEMP_MIN = 25.0;  // Minimum temperature threshold
        const float TEMP_MAX = 30.0;  // Maximum temperature threshold
        const float PH_MIN = 6.0;     // Minimum pH threshold
        const float PH_MAX = 8.0;     // Maximum pH threshold

        // Read temperature
        float temperature = readTemperature();

        // Read pH
        float pH = readpH();

        // Print temperature and pH values
        //Serial.print("Temperature: ");
        //Serial.print(temperature);
        //Serial.println(" °C");

        //Serial.print("pH Value: ");
        //Serial.println(pH);
        rotateServo();

        // Check temperature range
        if (temperature < TEMP_MIN) {
          // Turn on relay1 (heating coil)
          digitalWrite(RELAY1_PIN, HIGH);
        } else {
          // Turn off relay1 (heating coil)
          digitalWrite(RELAY1_PIN, LOW);
        }

        // Check pH range
        if (pH < PH_MIN || pH > PH_MAX) {
          // Turn on relay2 (pH-related action)
          digitalWrite(RELAY2_PIN, HIGH);
        } else {
          // Turn off relay2 (pH-related action)
          digitalWrite(RELAY2_PIN, LOW);
        }
        Serial.print("The Black and White Fish is Injured. Please take the required action. \n");
    }
  }

  // Rotate servo motor if 30 seconds have passed
  unsigned long currentMillis = millis();
  if (currentMillis - lastServoTime >= SERVO_PERIOD) {
    rotateServo();
    lastServoTime = currentMillis;
  }
}

float readTemperature() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

float readpH() {
  int rawpH = analogRead(A0);
  float volt = rawpH * 5.0 / 1024 / 6;
  float pH = -5.70 * volt + calibration_value;
  return pH;
}

void rotateServo() {
  // Rotate servo motor to 180 degrees
  servo.write(180);
  delay(1000);

  // Rotate servo motor back to 0 degrees
  servo.write(0);
  delay(1000);
}
