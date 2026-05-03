#ifndef TMC2209_H
#define TMC2209_h

#define ENABLE_PIN 25
#define STEP_PIN 26
#define DIR_PIN 27
#define SWITCH_RIGHT_PIN 33
#define SWITCH_LEFT_PIN 32

// 360 degrees / 1600 steps
#define ANGLE_PER_STEP 0.225

#include "ALBERT_SWITCHES.h"

void initializeTMC2209();
void initializeStepperMotor();

void rotateStepperMotor(float angle);
float convertAngle(float angle);

void calibrateStepperMotor(float angle);
void resetCurrentAngle();

extern float currentAngle;

#endif