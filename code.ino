// ---------------- MOTORS ----------------
int IN1 = 8;
int IN2 = 9;
int ENA = 5;

int IN3 = 10;
int IN4 = 11;
int ENB = 6;

// ---------------- ULTRASONIC ----------------
int TRIG_L = 2;
int ECHO_L = 3;

int TRIG_R = 4;
int ECHO_R = 7;

// ---------------- IR ----------------
int IR_L = A0;
int IR_R = A1;

// ---------------- SPEED ----------------
int baseSpeed = 100;


int trashScore = 0;
int obstacleScore = 0;

// ---------------- DISTANCE FUNCTION ----------------
long getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 20000);
  long distance = duration * 0.034 / 2;

  if (distance == 0) distance = 200;

  return distance;
}

// ---------------- MOVEMENT ----------------
void forward(int spd) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}

void backward(int spd) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, baseSpeed);
  analogWrite(ENB, baseSpeed);
  delay(350);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, baseSpeed);
  analogWrite(ENB, baseSpeed);
  delay(350);
}

// ---------------- SETUP ----------------
void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);

  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);

  pinMode(IR_L, INPUT);
  pinMode(IR_R, INPUT);
}

// ---------------- LOOP ----------------
void loop() {

  int distL = getDistance(TRIG_L, ECHO_L);
  int distR = getDistance(TRIG_R, ECHO_R);

  int frontDist = min(distL, distR);
  int diff = abs(distL - distR);

  // ---- CLASSIFICATION ZONE ----
  if (frontDist < 25) {

    // ---- WEIGHTING SYSTEM ----
    if (diff > 10) {
      trashScore++;
      obstacleScore = max(0, obstacleScore - 1);
    } else {
      obstacleScore++;
      trashScore = max(0, trashScore - 1);
    }

    // ---- DECISION ----
    if (frontDist < 12) {

      // 🗑️ TRASH
      if (trashScore > 3) {
        stopMotors();
        delay(200);

        backward(baseSpeed);
        delay(400);

        stopMotors();
        delay(3000); // placeholder

        trashScore = 0;
        obstacleScore = 0;
        return;
      }

      // 🧱 OBSTACLE
      else if (obstacleScore > 3) {
        stopMotors();
        delay(150);

        if (distL > distR) turnLeft();
        else turnRight();

        trashScore = 0;
        obstacleScore = 0;
        return;
      }
    }

    // ---- APPROACH ----
    forward(80);
    delay(50);
    return;
  }

  // ---- RESET SCORES WHEN NOTHING NEAR ----
  trashScore = 0;
  obstacleScore = 0;

  // ---- NORMAL ----
  forward(baseSpeed);
  delay(50);
}