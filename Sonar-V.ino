/**
 * Project name: Sonar-V
 * Author: Allan Brazute Alves
 * 
 * What it takes:
 * 1- Arduino (UNO R3)
 * 2- Steeper (28BYJ-48 - Pins IN1:8, IN2:10, IN3:9, IN4:11 - 5V)
 * 3- Ultrasonic range finder (HC-SRD4 - Pins E:12, T:13 - 5V)
 * 4- Buzzer (Pin 3)
 * 
 */

#include "Ultrasonic.h"
#include <Stepper.h>
#include <math.h>

// Constants
const byte BUZZER_PIN = 3;
const byte BUZZER_VOL = 100;

const byte STEPPER_PIN[] = {8, 10, 9, 11};
const byte STEPPER_RPM = 64;
const int STEPPER_SPEED = 100;
const int STEPPER_STEPS2GO = 20; //2048;
const int STEPPER_ARC = 128;

const byte USONIC_PIN[] = {12,13};

const int SERIAL_BRATE = 9600;

// Variables;
byte minDistDiff = 3;
int maxDistDiff = 1000;
int l = 0;
int r = 0;

int radar[2][round((STEPPER_ARC/STEPPER_STEPS2GO)+1)];
int countPos = 0;
int motorPos = 0;
int motorDir = 1; //1 Right, -1 Left

// Instantiate classes
Stepper stepper(STEPPER_RPM, STEPPER_PIN[0], STEPPER_PIN[1], STEPPER_PIN[2], STEPPER_PIN[3]);
Ultrasonic ultrasonic(USONIC_PIN[0], USONIC_PIN[1]); //(Trig,Echo);


void setup()
{
  stepper.setSpeed(STEPPER_SPEED);
  Serial.begin(SERIAL_BRATE);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
  if (motorDir < 0) 
  {
    l = radar[0][countPos] = ultrasonic.Ranging(CM);
  }
  else
  {
    r = radar[1][countPos] = ultrasonic.Ranging(CM);
  }
  
  int lrDiff = (l > r)? l - r : r - l;
  
  Serial.println(String(motorPos) + ": " + String(countPos) + ": " + String(l) + ": " + String(r) + ": " + String(lrDiff) + ": " + String(motorDir));
  
  if ( lrDiff > minDistDiff && lrDiff < maxDistDiff )
  {
    //Serial.println(motorPos + txt + countPos + txt + l + txt + r + txt + lrDiff + txt + motorDir);
    
    analogWrite(BUZZER_PIN, BUZZER_VOL);
    delay(lrDiff);
    digitalWrite(BUZZER_PIN, LOW);
  }
  
  motorStep();
}

void motorStep()
{
  if ( (motorDir == -1 && motorPos <= (STEPPER_ARC - STEPPER_ARC)) || (motorDir == 1 && motorPos >= STEPPER_ARC) )
  {
    motorDir *= -1;
  }

  int stepNow = STEPPER_STEPS2GO * motorDir;
  
  stepper.step(stepNow);
  motorPos += stepNow;
  countPos += motorDir;
}
