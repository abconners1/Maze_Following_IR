/* A closed loop proportional control. Use 2 break beam sensors and
 * interrupt to count the encoder on the wheels of the robot.
 * Yer Yang and Andrew Conners
 * 3/8/2017
*/
#include <PinChangeInt.h>

// these next two are the digital pins we'll use for the encoders
#define  EncoderMotorLeft  8
#define  EncoderMotorRight 7
#define  GAINleftMotor  (1)
#define  GAINrightMotor (1)
#define  LEFTmotor     1
#define  RIGHTmotor    2
// here we define the pwm and direction for Right Motor (A) and Left Motor (B)
#define  motorRightPWM        3
#define  motorRightDirection 12
#define  motorLeftPWM       11
#define  motorLeftDirection 13
#define  threshold (120)
float distance = 26.48;
int leftIR;
int forwardIR;
#define  F (0)
#define  L (1)
#define  R (2)
int i = 0;
int turnArray[50]={};


// Variables for the interrupts
volatile unsigned int leftEncoderCount = 0;
volatile unsigned int rightEncoderCount = 0; 

void setup(){
  // set stuff up
  Serial.begin(9600);
  Serial.print("Encoder Testing Program ");
  Serial.print("Now setting up the Left Encoder: Pin ");
  Serial.print(EncoderMotorLeft);
  Serial.println();
  pinMode(EncoderMotorLeft, INPUT);     //set the pin to input
  digitalWrite(EncoderMotorLeft, HIGH); //use the internal pullup resistor
  // this next line setup the PinChange Interrupt 
  PCintPort::attachInterrupt(EncoderMotorLeft, indexRightEncoderCount,CHANGE);
  /////////////////////////////////////////////////
  Serial.print("Now setting up the Right Encoder: Pin ");
  Serial.print(EncoderMotorRight);
  Serial.println();
  pinMode(EncoderMotorRight, INPUT);     //set the pin to input
  digitalWrite(EncoderMotorRight, HIGH); //use the internal pullup resistor
  //we could have set this up using just "pinMode(EncoderMotorRight, INPUT_PULLUP);
  PCintPort::attachInterrupt(EncoderMotorRight, indexLeftEncoderCount,CHANGE);
  delay(10000);
} /////////////// end of setup ////////////////////////////////////

/////////////////////// loop() ////////////////////////////////////
void loop()
{ 
 // In each loop, we access the index in the array. 
  leftIR = analogRead(1);
  forwardIR = analogRead(0);
  exploreMaze(leftIR, forwardIR);

}
//////////////////////////////// end of loop() /////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Drive function
int drive(float distance)
{
  int countLeft = 0;
  int countRight = 0;
  leftEncoderCount = 0;
  rightEncoderCount = 0;
  leftIR = analogRead(1);
  // Stay in this loop until desired encoder counts has been reached
  while(leftIR >= threshold)
   {
    motor(LEFTmotor, 70); // call the motor function
    motor(RIGHTmotor, 100); // call the motor function 
    countLeft = leftEncoderCount;
    countRight = rightEncoderCount;
    leftIR = analogRead(1);
    forwardIR = analogRead(0);
    if ( leftIR > 400 || forwardIR >= 420){
      if (forwardIR >= 420){
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
      else if (leftIR > 400 || forwardIR >= 420){
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
        delay(500);
        analogWrite(motorRightPWM, 0);
        analogWrite(motorLeftPWM, 0);
        delay(400);
      }
    }
   }  
}

void motor(int m, int pwm){
  Serial.print("entering motor");
  Serial.print(m);
  Serial.print("\t");
  Serial.println(pwm);

  
  if(m == LEFTmotor){

    if(pwm > 0){
    digitalWrite(motorLeftDirection, HIGH);
    analogWrite(motorLeftPWM, pwm);
    }
    else if(pwm < 0){
      digitalWrite(motorLeftDirection, LOW);
      analogWrite(motorLeftPWM, -pwm);
      }
    else{
      analogWrite(motorLeftPWM, 0);
    }
  }
  /////////////////////////// end of left motor //////////////
  /////// now right motor //////////////
    if(m == RIGHTmotor){
      if(pwm > 0){
      digitalWrite(motorRightDirection, HIGH);
      analogWrite(motorRightPWM, pwm);
      }
      else if(pwm < 0){
        digitalWrite(motorRightDirection, LOW);
        analogWrite(motorRightPWM, -pwm);
      }
      else {
        analogWrite(motorRightPWM, 0);
      }
    }////////////////////////////// end of right motor //////////////
  }

////////////////////////////////////////////////////// 
int exploreMaze(int leftIR, int forwardIR)
{
  if (leftIR > threshold && forwardIR < threshold){
    drive(distance);
    storeToArray(F);
    goForward();
    storeToArray(L);
  }

}
///////////////////////////////////////////////////////////////////////////
void storeToArray(int turn)
{
  if (turn == F)
    turnArray[i]=F;
  else if ( turn == L)
    turnArray[i] = L;
  else if (turn == R)
    turnArray[i] = R;
  i+1;
}
/////////////////////////////////////////////////////////////////////
void goForward()
{
  while (leftIR <= 420){
    digitalWrite(motorRightDirection, HIGH);
    analogWrite(motorRightPWM, 100);
    digitalWrite(motorLeftDirection, HIGH);
    analogWrite(motorLeftPWM, 50);
    leftIR = analogRead(1);
    forwardIR = analogRead(0);
    if (leftIR > 420 || forwardIR >= 450){
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
      delay(500);
      analogWrite(motorRightPWM, 0);
      analogWrite(motorLeftPWM, 0);
      delay(400);
    }
  }
}
///////////////////////////////////////////////////////////////////////
void indexLeftEncoderCount()
{
  leftEncoderCount++;
}
//////////////////////////////////////////////////////////
void indexRightEncoderCount()
{
  rightEncoderCount++;
}
