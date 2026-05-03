#include "TMC2209.h"
#include <Arduino.h>

// Parallel programming (non-blocking) variables
unsigned long lastMillisReadData = 0;
unsigned long lastMicrosWriteState = 0;
bool writeState = HIGH;

float currentAngle = 0;
bool direction = 1;

// Switches used for calibration
int rightSwitchPressed = 0;
int leftSwitchPressed = 0;
float leftMaxValue;
float rightMaxValue;

// Initialize stepper motor driver tmc2209
void initializeTMC2209() {

  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);

  // Enable stepper mode driver
  digitalWrite(ENABLE_PIN, LOW);

  // Motor direction (clockwise)
  digitalWrite(DIR_PIN, direction);

}

// Find the starting point of the stepper motor and calibrate it
void initializeStepperMotor() {

  // Move left until press the left switch, (indicates the maximum value)
  while (!leftSwitchPressed) {

    Serial.println(currentAngle);
    // Make a circle in that direction until the max value is found
    calibrateStepperMotor(360.0);

    int leftSwitch = isSwitchPressed(SWITCH_LEFT_PIN);
    
    if (leftSwitch) {
      leftSwitchPressed = 1;
      leftMaxValue = currentAngle;
    }
  }

  // Move right until press the right switch, (indicates the maximum value)
  while (!rightSwitchPressed) {

    Serial.println(currentAngle);

    // Make a circle in that direction until the max value is found
    calibrateStepperMotor(-360);

    int rightSwitch = isSwitchPressed(SWITCH_RIGHT_PIN);
    
    if (rightSwitch) {
      rightSwitchPressed = 1;
      rightMaxValue = currentAngle;
    }
  }

  if (currentAngle < 0) {
    currentAngle += 360;
  }
  float startPoint = (leftMaxValue + rightMaxValue) / 2;

  Serial.print("START POINT ");
  Serial.println(startPoint);

  while (abs(currentAngle - startPoint) >= 0.5) {
    rotateStepperMotor(startPoint);
  }
  Serial.println(currentAngle);
  resetCurrentAngle();
}


// Rotate the stepper motor using pulses
// @param targetAngle the angle provided by AS5600 magnetic encoder. Angle range 0-360 degrees.
void rotateStepperMotor(float targetAngle) {

    // Angle difference
    float angleOffset = round((targetAngle - currentAngle) * 2.0) / 2.0;

    angleOffset = convertAngle(angleOffset);
    int steps = round((angleOffset / ANGLE_PER_STEP));
    
    // Change direction if needed
    if (steps >= 0) {
        if (direction == LOW) {
            direction = HIGH;
            digitalWrite(DIR_PIN, direction);
        }
    }
    else {
        if (direction == HIGH) {
            direction = LOW;
            digitalWrite(DIR_PIN, direction);
        }
    } 

    steps = abs(steps);

    // Rotate stepper motor if steps are provided
    if (steps != 0) {

            digitalWrite(STEP_PIN, HIGH);
            delayMicroseconds(1000);

            digitalWrite(STEP_PIN, LOW);
            delayMicroseconds(1000);

            currentAngle += ANGLE_PER_STEP * (angleOffset/abs(angleOffset));
    }
}


// Converts given angle into range of -180 and 180 degrees.
// @param angle angle to be converted
// @returns new converted angle
float convertAngle(float angle) {

    if (abs(angle) > 180) {
        return ((360 - abs(angle))*(-(angle/abs(angle))));
    }

    return angle;
}


// Calibrate the stepper motor using pulses.
// @param angle in range of -360 and 360 degrees
void calibrateStepperMotor(float angle) {

    // Angle difference
    float angleOffset = (angle - currentAngle);

    int steps = round((angleOffset / ANGLE_PER_STEP));
    
    // Change direction if needed
    if (steps >= 0) {
        if (direction == LOW) {
            direction = HIGH;
            digitalWrite(DIR_PIN, direction);
        }
    }
    else {
        if (direction == HIGH) {
            direction = LOW;
            digitalWrite(DIR_PIN, direction);
        }
    } 

    steps = abs(steps);

    // Rotate stepper motor if steps are provided
    if (steps != 0) {

            digitalWrite(STEP_PIN, HIGH);
            delayMicroseconds(1000);

            digitalWrite(STEP_PIN, LOW);
            delayMicroseconds(1000);

            currentAngle += ANGLE_PER_STEP * (angleOffset/abs(angleOffset));
    }
}

// Reset angle
void resetCurrentAngle() {
    currentAngle = 0;
}