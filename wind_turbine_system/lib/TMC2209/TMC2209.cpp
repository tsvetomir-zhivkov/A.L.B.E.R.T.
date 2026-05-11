#include "TMC2209.h"
#include <Arduino.h>
#include <math.h>

float currentOffset = 0;
float currentAngle = 0;
bool direction = HIGH;

// Switches used for calibration
int rightSwitchPressed = 0;
int leftSwitchPressed = 0;
// Constraints in the zones of the rotor's moving range (circle)
float leftMaxAngle, rightMaxAngle;

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

  float leftOffset, rightOffset;
  // Move left until press the left switch, (indicates the maximum value)
  while (!leftSwitchPressed) {
    // Make a circle in that direction until the max value is found
    calibrateStepperMotor(-360.0);

    int leftSwitch = isSwitchPressed(SWITCH_LEFT_PIN);
    
    if (leftSwitch) {
      leftSwitchPressed = 1;
      leftOffset = currentOffset;
    }
  }

  // Move right until press the right switch, (indicates the maximum value)
  while (!rightSwitchPressed) {
    // Make a circle in that direction until the max value is found
    calibrateStepperMotor(360);

    int rightSwitch = isSwitchPressed(SWITCH_RIGHT_PIN);
    
    if (rightSwitch) {
      rightSwitchPressed = 1;
      rightOffset = currentOffset;
    }
  }

  // Calculate the new start point of the rotor
  float startPointOffset = (leftOffset + rightOffset) / 2;
  //float startPoint = convertAngle(startPointOffset);

  // Rotate the stepper motor to the new initial point
  while (abs(currentOffset - startPointOffset) >= ANGLE_PER_STEP) {
    calibrateStepperMotor(startPointOffset);
  }

  // Calculate the maximum offsets of the new starting point
  float leftMaxOffset = leftOffset - startPointOffset;
  float rightMaxOffset = rightOffset - startPointOffset;

  // Calculate the maximum angles
  leftMaxAngle = convertToAngle(leftMaxOffset);
  rightMaxAngle = convertToAngle(rightMaxOffset);

  Serial.printf("LEFT: %f, RIGHT: %f", leftMaxAngle, rightMaxAngle);
  // Reset the status of the switches
  leftSwitchPressed = 0;
  rightSwitchPressed = 0;

  // Reset current angle's values
  currentOffset = 0;
  currentAngle = 0;
}

// Rotate the stepper motor using pulses
// @param targetAngle the angle provided by AS5600 magnetic encoder. Angle range 0-360 degrees.
void rotateStepperMotor(float targetAngle) {

    // Calculate the offset from the current stepper motor's position
    float angleOffset = calculateOffset(&targetAngle);

    // Calculate the steps
    int steps = round((angleOffset / ANGLE_PER_STEP));
    
    // Change direction if required
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

    // Rotate the stepper motor using a pulse
    if (steps != 0) {

            digitalWrite(STEP_PIN, HIGH);
            delayMicroseconds(1000);

            digitalWrite(STEP_PIN, LOW);
            delayMicroseconds(1000);

            // Change the current stepper motor's position with one step
            currentOffset += ANGLE_PER_STEP * (angleOffset/abs(angleOffset));
            // Convert the offset to a circle angle in range 0-360 degrees
            currentAngle = convertToAngle(currentOffset);
    }
}


// Calculate the offset from the current stepper motor's position
// @param *targetAngle a pointer to the provided target angle
// @returns the difference between the stepper motor's position (angle) and target position (angle) provided by the AS5600
float calculateOffset(float *targetAngle) {

    CircleZone targetZone;
    CircleZone currentZone;

    // Check in which zone is the target angle
    // Left zone (before triggering left switch)
    if (*targetAngle >= 0 && *targetAngle < rightMaxAngle) {
        targetZone = LEFT_ZONE;
    }
    // Right zone (before triggering right switch)
    else if (*targetAngle > leftMaxAngle && *targetAngle <= 360) {
        targetZone = RIGHT_ZONE;
    }
    // Danger zone (between switches)
    else {
        // Check which constrain is closer to the target angle
        float middlePoint = (leftMaxAngle + rightMaxAngle) / 2;
        if (*targetAngle >= middlePoint) {
            *targetAngle = leftMaxAngle;
            targetZone = RIGHT_ZONE;
        }
        else {
            *targetAngle = rightMaxAngle;
            targetZone = LEFT_ZONE;
        }
    }

    //printf("TARGET ANGLE: %f\n", *targetAngle);

    // Check in which zone is the stepper motor's current angle (assuming that the current position cannot be in the danger zone)
    if (currentAngle >= 0 && currentAngle < leftMaxAngle) {
        currentZone = LEFT_ZONE;
    }
    // Right zone (before triggering right switch)
    else if (currentAngle > rightMaxAngle && currentAngle <= 360) {
        currentZone = RIGHT_ZONE;
    }

    // Calculate the offset (difference between target angle and stepper motor's current angle/position)
    if (targetZone == currentZone) {
        return round((*targetAngle - currentAngle) * 2.0) / 2.0;
    }
    else if (targetZone > currentZone) {
        return -((360-*targetAngle) + currentAngle);
    }
    else {
        return (360 - currentAngle) + *targetAngle;
    }

}

// Convert the provided offset to a positive angle in range 0-360 degrees
// @param offset either positive or negative offset
// @return the converted angle in range 0-360 degrees
float convertToAngle(float offset) {

    // Map the offset in range -360 - 360 degrees
    offset = fmod(offset, 360.0f);

    // If the offset is negative, make it positive
    if (offset < 0) {
        offset += 360;
    }

    return offset;
}

// Calibrate the stepper motor (this function works the same way as rotateStepperMotor() function).
// The only difference is that this function uses only offsets and the other uses both offsets and angles.
// @param offset an offset in range of -360 and 360 degrees
void calibrateStepperMotor(float offset) {

    // Calculate offset (angle difference)
    float angleOffset = (offset - currentOffset);

    // Calculate the steps
    int steps = round((angleOffset / ANGLE_PER_STEP));
    
    // Change direction if required
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

    // Rotate stepper motor using a pulse
    if (steps != 0) {

            digitalWrite(STEP_PIN, HIGH);
            delayMicroseconds(1000);

            digitalWrite(STEP_PIN, LOW);
            delayMicroseconds(1000);

            // Change the current stepper motor's position with one step
            currentOffset += ANGLE_PER_STEP * (angleOffset/abs(angleOffset));
    }
}

