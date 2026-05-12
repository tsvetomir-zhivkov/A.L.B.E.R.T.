#ifndef TMC2209_H
#define TMC2209_h

#define ENABLE_PIN 25
#define STEP_PIN 26
#define DIR_PIN 27

// 360 degrees / 1600 steps
#define ANGLE_PER_STEP 0.225

#include "ALBERT_SWITCHES.h"

enum CircleZone {
    LEFT_ZONE,
    RIGHT_ZONE
};

// Initialization functions
void initializeTMC2209();
void initializeStepperMotor();

// Rotate stepper motor
void rotateStepperMotor(float angle);
void calibrateStepperMotor(float angle);

// Conversion functions
int convertToAngle(float offset);
float calculateOffset(float *targetAngle);


extern int currentAngle;

#endif