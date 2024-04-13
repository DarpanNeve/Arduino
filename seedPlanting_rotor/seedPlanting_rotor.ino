#define in1 9
#define in2 8
#define en 10
char val;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(en, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(en, 0);
  digitalWrite(en, LOW);
  delay(1000);
}

void loop() {
  if (Serial.available() > 0) {
    val = Serial.read();
    if (val == '1') {
      Serial.println("Wheat");
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      analogWrite(en, 255);
    } else if (val == '2') {
      Serial.println("Pulse");
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      analogWrite(en, 255);
      delay(1500);
    }
  }
}