#include "TMC2209.h"
#include <Arduino.h>

// Parallel programming (non-blocking) variables
unsigned long lastMillisReadData = 0;
unsigned long lastMicrosWriteState = 0;
bool writeState = HIGH;

float currentAngle = 0;
bool direction = 1;


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

// Rotate the stepper motor using pulses
// @param targetAngle the angle provided by AS5600 magnetic encoder 
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

