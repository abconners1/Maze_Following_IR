/* 
 * A closed-loop proportional control implementation for a mobile robot.
 * Utilizes two break beam sensors for wheel encoders and IR sensors
 * to explore and solve a maze.
 * 3/8/2017
 */

#include <PinChangeInt.h> // Library for handling pin change interrupts

// Digital pins assigned for wheel encoders
#define EncoderMotorLeft 8
#define EncoderMotorRight 7

// Gain factors for left and right motors
#define GAINleftMotor (1)
#define GAINrightMotor (1)

// Motor identifiers
#define LEFTmotor 1
#define RIGHTmotor 2

// PWM and direction pins for the motors
#define motorRightPWM 3
#define motorRightDirection 12
#define motorLeftPWM 11
#define motorLeftDirection 13

// Threshold for IR sensor readings
#define threshold (120)

// Constants for direction encoding
#define F (0) // Forward
#define L (1) // Left turn
#define R (2) // Right turn

// Variables for IR sensor readings and maze navigation
float distance = 26.48; // Default cell distance to traverse
int leftIR;            // Left IR sensor reading
int forwardIR;         // Forward IR sensor reading

// Array to store movement commands for optimization
int turnArray[50] = {};
int i = 0; // Index for turnArray

// Encoder count variables (updated via interrupts)
volatile unsigned int leftEncoderCount = 0;
volatile unsigned int rightEncoderCount = 0;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Encoder Testing Program");

  // Setup for the left encoder
  Serial.print("Now setting up the Left Encoder: Pin ");
  Serial.println(EncoderMotorLeft);
  pinMode(EncoderMotorLeft, INPUT);     // Configure as input
  digitalWrite(EncoderMotorLeft, HIGH); // Enable pull-up resistor
  PCintPort::attachInterrupt(EncoderMotorLeft, indexLeftEncoderCount, CHANGE); // Attach interrupt

  // Setup for the right encoder
  Serial.print("Now setting up the Right Encoder: Pin ");
  Serial.println(EncoderMotorRight);
  pinMode(EncoderMotorRight, INPUT);    // Configure as input
  digitalWrite(EncoderMotorRight, HIGH); // Enable pull-up resistor
  PCintPort::attachInterrupt(EncoderMotorRight, indexRightEncoderCount, CHANGE); // Attach interrupt

  // Delay for stabilization
  delay(10000);
}

void loop() {
  // Continuously read sensor data and explore the maze
  leftIR = analogRead(1);   // Left IR sensor reading
  forwardIR = analogRead(0); // Forward IR sensor reading
  exploreMaze(leftIR, forwardIR); // Call the maze exploration function
}

// Drive function to move the robot a specified distance
int drive(float distance) {
  int countLeft = 0;
  int countRight = 0;

  // Reset encoder counts
  leftEncoderCount = 0;
  rightEncoderCount = 0;

  // Loop until the desired encoder counts are reached
  while (leftIR >= threshold) {
    motor(LEFTmotor, 70);  // Set left motor speed
    motor(RIGHTmotor, 100); // Set right motor speed

    // Update encoder counts
    countLeft = leftEncoderCount;
    countRight = rightEncoderCount;

    // Update IR sensor readings
    leftIR = analogRead(1);
    forwardIR = analogRead(0);

    // Obstacle detection and handling
    if (leftIR > 400 || forwardIR >= 420) {
      // Handle turning maneuvers based on sensor inputs
      turnObstacleHandling(forwardIR);
    }
  }
}

// Function to control motors based on input parameters
void motor(int m, int pwm) {
  if (m == LEFTmotor) {
    if (pwm > 0) {
      digitalWrite(motorLeftDirection, HIGH);
      analogWrite(motorLeftPWM, pwm);
    } else if (pwm < 0) {
      digitalWrite(motorLeftDirection, LOW);
      analogWrite(motorLeftPWM, -pwm);
    } else {
      analogWrite(motorLeftPWM, 0);
    }
  } else if (m == RIGHTmotor) {
    if (pwm > 0) {
      digitalWrite(motorRightDirection, HIGH);
      analogWrite(motorRightPWM, pwm);
    } else if (pwm < 0) {
      digitalWrite(motorRightDirection, LOW);
      analogWrite(motorRightPWM, -pwm);
    } else {
      analogWrite(motorRightPWM, 0);
    }
  }
}

// Function to explore the maze based on IR sensor data
int exploreMaze(int leftIR, int forwardIR) {
  if (leftIR > threshold && forwardIR < threshold) {
    drive(distance);      // Drive forward
    storeToArray(F);      // Log forward move
    goForward();          // Continue forward
    storeToArray(L);      // Log left turn
  }
}

// Function to store movement commands in an array
void storeToArray(int turn) {
  if (turn == F) {
    turnArray[i] = F;
  } else if (turn == L) {
    turnArray[i] = L;
  } else if (turn == R) {
    turnArray[i] = R;
  }
  i++; // Increment array index
}

// Function to move the robot forward until a condition is met
void goForward() {
  while (leftIR <= 420) {
    motor(RIGHTmotor, 100); // Move right motor forward
    motor(LEFTmotor, 50);   // Move left motor forward

    // Update IR sensor readings
    leftIR = analogRead(1);
    forwardIR = analogRead(0);

    // Handle obstacle detection during forward motion
    if (leftIR > 420 || forwardIR >= 450) {
      turnObstacleHandling(forwardIR);
    }
  }
}

// Interrupt handler for the left encoder
void indexLeftEncoderCount() {
  leftEncoderCount++;
}

// Interrupt handler for the right encoder
void indexRightEncoderCount() {
  rightEncoderCount++;
}

// Function to handle obstacles during navigation
void turnObstacleHandling(int forwardIR) {
  digitalWrite(motorRightDirection, LOW);
  analogWrite(motorRightPWM, 80);
  digitalWrite(motorLeftDirection, LOW);
  analogWrite(motorLeftPWM, 80);
  delay(500);

  analogWrite(motorRightPWM, 0);
  analogWrite(motorLeftPWM, 0);
  delay(400);

  digitalWrite(motorRightDirection, LOW);
  analogWrite(motorRightPWM, 80);
  digitalWrite(motorLeftDirection, HIGH);
  analogWrite(motorLeftPWM, 80);
  delay(800);

  analogWrite(motorRightPWM, 0);
  analogWrite(motorLeftPWM, 0);
  delay(400);
  storeToArray(R);
}
