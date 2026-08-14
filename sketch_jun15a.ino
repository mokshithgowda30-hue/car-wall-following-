// ============================================================
// CAR4 - OUTSIDE SQUARE WALL FOLLOWER
// LEFT WALL FOLLOWING
// Arduino UNO
//
// CAR IS OUTSIDE THE SQUARE
// WALL IS ON THE LEFT SIDE
//
// Sensors:
// A0 = Front Left
// A1 = Front Right
// A2 = Left
// A3 = Right
//
// Wall = LOW value (~0-15)
// Clear = HIGH value (~620)
// ============================================================


// ============================================================
// MOTOR PINS
// ============================================================

#define IN1 3
#define IN2 4
#define ENA 5

#define IN3 8
#define IN4 7
#define ENB 6


// ============================================================
// SENSOR PINS
// ============================================================

#define FRONT_LEFT  A0
#define FRONT_RIGHT A1
#define LEFT_SENSOR A2
#define RIGHT_SENSOR A3


// ============================================================
// SENSOR THRESHOLD
// ============================================================

#define WALL_THRESHOLD 300


// ============================================================
// MOTOR SPEED
// ============================================================

#define BASE_SPEED 55

// Motor compensation
#define LEFT_BASE_SPEED  55
#define RIGHT_BASE_SPEED 55


// ============================================================
// WALL FOLLOWING CORRECTION
// ============================================================
//
// Because your IR sensors give approximately:
//
// Very close wall = 0-10
// Far/clear       = 620
//
// We use three zones instead of trying to calculate
// an exact distance.
//
// ------------------------------------------------------------
//
// LEFT sensor LOW:
// Car is close to wall
// -> move RIGHT
//
// LEFT sensor HIGH:
// Car is far from wall
// -> move LEFT
//
// ============================================================

#define VERY_CLOSE  8
#define WALL_NEAR   80
#define WALL_FAR    300


// ============================================================
// CORRECTION SPEEDS
// ============================================================

#define CORRECT_FAST 65
#define CORRECT_SLOW 40


// ============================================================
// TURN SPEED
// ============================================================

#define TURN_SPEED 55


// ============================================================
// TURN PARAMETERS
// ============================================================

#define MIN_TURN_TIME 300
#define MAX_TURN_TIME 1000


// ============================================================
// SENSOR VARIABLES
// ============================================================

int FL;
int FR;
int L;
int R;


// ============================================================
// SETUP
// ============================================================

void setup()
{
  Serial.begin(9600);

  // ----------------------------------------------------------
  // MOTOR PINS
  // ----------------------------------------------------------

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);


  stopMotors();

  Serial.println();
  Serial.println("========================================");
  Serial.println(" CAR4 OUTSIDE SQUARE WALL FOLLOWER");
  Serial.println("          LEFT WALL MODE");
  Serial.println("========================================");

  Serial.println();
  Serial.println("Starting in 3 seconds...");

  delay(3000);

  Serial.println("START");
}


// ============================================================
// MAIN LOOP
// ============================================================

void loop()
{
  readSensors();


  // ==========================================================
  // FRONT WALL DETECTION
  // ==========================================================

  bool frontLeftBlocked =
      FL < WALL_THRESHOLD;

  bool frontRightBlocked =
      FR < WALL_THRESHOLD;

  bool frontBlocked =
      frontLeftBlocked && frontRightBlocked;


  // ==========================================================
  // LEFT WALL DETECTION
  // ==========================================================

  bool leftWall =
      L < WALL_THRESHOLD;


  // ==========================================================
  // PRINT VALUES
  // ==========================================================

  Serial.print("FL=");
  Serial.print(FL);

  Serial.print(" FR=");
  Serial.print(FR);

  Serial.print(" L=");
  Serial.print(L);

  Serial.print(" R=");
  Serial.print(R);

  Serial.print(" -> ");


  // ==========================================================
  // 1. FRONT BLOCKED
  // ==========================================================
  //
  // We have reached a corner.
  //
  // Since the square is on our LEFT side, turn LEFT.
  //
  // ==========================================================

  if (frontBlocked)
  {
    Serial.println("CORNER -> LEFT TURN");

    turnLeft();

    return;
  }


  // ==========================================================
  // 2. LEFT WALL COMPLETELY LOST
  // ==========================================================
  //
  // This means the car has reached an outside corner
  // or has moved too far away from the wall.
  //
  // Search toward LEFT.
  //
  // ==========================================================

  if (L > WALL_FAR)
  {
    Serial.println("LEFT WALL LOST -> SEARCH LEFT");

    searchLeft();

    return;
  }


  // ==========================================================
  // 3. CAR TOO CLOSE TO WALL
  // ==========================================================
  //
  // L is very low.
  //
  // Move slightly AWAY from wall.
  //
  // Left wheel faster
  // Right wheel slower
  //
  // This curves the car toward RIGHT.
  //
  // ==========================================================

  if (L < VERY_CLOSE)
  {
    Serial.println("TOO CLOSE -> MOVE RIGHT");

    steerRight();

    return;
  }


  // ==========================================================
  // 4. CAR NEAR WALL
  // ==========================================================
  //
  // Normal forward motion.
  //
  // ==========================================================

  if (L < WALL_NEAR)
  {
    Serial.println("WALL OK -> FORWARD");

    moveForward();

    return;
  }


  // ==========================================================
  // 5. WALL IS SOMEWHAT FAR
  // ==========================================================
  //
  // Curve gently toward LEFT.
  //
  // ==========================================================

  Serial.println("WALL FAR -> MOVE LEFT");

  steerLeft();
}


// ============================================================
// READ SENSORS
// ============================================================

void readSensors()
{
  FL = analogRead(FRONT_LEFT);
  FR = analogRead(FRONT_RIGHT);

  L = analogRead(LEFT_SENSOR);
  R = analogRead(RIGHT_SENSOR);
}


// ============================================================
// FORWARD
// ============================================================
//
// CONFIRMED FORWARD DIRECTION FROM YOUR MOTOR TEST
// ============================================================

void moveForward()
{
  // LEFT MOTOR
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // RIGHT MOTOR
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, LEFT_BASE_SPEED);
  analogWrite(ENB, RIGHT_BASE_SPEED);

  delay(20);
}


// ============================================================
// STEER RIGHT
// ============================================================
//
// Wall is too close.
//
// Increase left speed.
// Decrease right speed.
//
// Car curves RIGHT away from wall.
// ============================================================

void steerRight()
{
  // LEFT MOTOR
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // RIGHT MOTOR
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, CORRECT_FAST);
  analogWrite(ENB, CORRECT_SLOW);

  delay(25);
}


// ============================================================
// STEER LEFT
// ============================================================
//
// Wall is too far.
//
// Decrease left speed.
// Increase right speed.
//
// Car curves LEFT toward wall.
// ============================================================

void steerLeft()
{
  // LEFT MOTOR
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // RIGHT MOTOR
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, CORRECT_SLOW);
  analogWrite(ENB, CORRECT_FAST);

  delay(25);
}


// ============================================================
// SEARCH LEFT
// ============================================================
//
// Left wall disappeared.
//
// Slowly curve LEFT until the sensor detects the wall again.
// ============================================================

void searchLeft()
{
  unsigned long startTime = millis();

  while (true)
  {
    readSensors();

    // LEFT MOTOR SLOW
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    // RIGHT MOTOR FAST
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, 35);
    analogWrite(ENB, 65);


    // --------------------------------------------------------
    // WALL FOUND
    // --------------------------------------------------------

    if (L < WALL_THRESHOLD)
    {
      stopMotors();

      delay(80);

      Serial.println("LEFT WALL FOUND");

      return;
    }


    // --------------------------------------------------------
    // SAFETY TIMEOUT
    // --------------------------------------------------------

    if (millis() - startTime > 800)
    {
      stopMotors();

      delay(50);

      return;
    }

    delay(10);
  }
}


// ============================================================
// LEFT TURN
// ============================================================
//
// Turn around an outside corner.
//
// LEFT wheel = reverse
// RIGHT wheel = forward
// ============================================================

void turnLeft()
{
  Serial.println("TURN LEFT");


  unsigned long startTime = millis();


  while (true)
  {
    // --------------------------------------------------------
    // LEFT MOTOR REVERSE
    // --------------------------------------------------------

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);


    // --------------------------------------------------------
    // RIGHT MOTOR FORWARD
    // --------------------------------------------------------

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);


    analogWrite(ENA, TURN_SPEED);
    analogWrite(ENB, TURN_SPEED);


    // --------------------------------------------------------
    // Wait minimum turning time
    // --------------------------------------------------------

    if (millis() - startTime > MIN_TURN_TIME)
    {
      readSensors();


      // New wall detected on left
      if (L < WALL_THRESHOLD)
      {
        stopMotors();

        delay(100);

        Serial.println("NEW WALL FOUND");

        return;
      }
    }


    // --------------------------------------------------------
    // SAFETY
    // --------------------------------------------------------

    if (millis() - startTime > MAX_TURN_TIME)
    {
      stopMotors();

      delay(100);

      return;
    }

    delay(10);
  }
}


// ============================================================
// STOP
// ============================================================

void stopMotors()
{
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}