// ============================================
// RED LED INDICATOR SENSOR (PIN A3) - RIGHT TURN
// RED LED ON  (HIGH) -> MOVE FRONT
// RED LED OFF (LOW)  -> ROTATE RIGHT
// ============================================

// Motor Control Pins (UNCHANGED)
#define IN1 3
#define IN2 4
#define ENA 5
#define IN3 8
#define IN4 7
#define ENB 6

// Active Sensor Pin (UNCHANGED)
#define ACTIVE_IR_PIN A3

// Motor Speeds (UNCHANGED)
#define SPEED_BASE 80
#define SPEED_TURN 85

void setup() {
  Serial.begin(9600);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  pinMode(ACTIVE_IR_PIN, INPUT);
  
  stopMotors();
  Serial.println(F("===================================="));
  Serial.println(F("✅ RED LED ON = FORWARD | OFF = RIGHT"));
  Serial.println(F("===================================="));
  delay(1000);
}

void loop() {
  // Read state from pin A3
  int sensorSignal = digitalRead(ACTIVE_IR_PIN);
  
  Serial.print(F("Pin A3 Signal: "));
  Serial.print(sensorSignal);

  // -------------------------------------------------------------
  // RED LED IS ON (sensorSignal == HIGH) -> MOVE FRONT
  // -------------------------------------------------------------
  if (sensorSignal == HIGH) {
    Serial.println(F(" | 🔴 RED LED ON  -> Moving FRONT"));
    goStraight();
  } 
  
  // -------------------------------------------------------------
  // RED LED IS OFF (sensorSignal == LOW) -> ROTATE RIGHT
  // -------------------------------------------------------------
  else {
    Serial.println(F(" | ⚪ RED LED OFF -> Rotating RIGHT"));
    turnRight();
  }
  
  delay(40);
}

// ========== MOTOR CONTROLS ==========

// Drive Straight Forward
void goStraight() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, SPEED_BASE); 
  analogWrite(ENB, SPEED_BASE);
}

// Spin Right in Place (Left wheel FORWARD, Right wheel REVERSES)
void turnRight() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); // LEFT MOTOR: FORWARD
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); // RIGHT MOTOR: REVERSE
  analogWrite(ENA, SPEED_TURN); 
  analogWrite(ENB, SPEED_TURN);
}

void stopMotors() {
  analogWrite(ENA, 0); 
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}
