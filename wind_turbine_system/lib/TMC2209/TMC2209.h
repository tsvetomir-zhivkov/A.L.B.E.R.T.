#ifndef TMC2209_H
#define TMC2209_h

#define ENABLE_PIN 25
#define STEP_PIN 26
#define DIR_PIN 27

// 360 degrees / 1600 steps
#define ANGLE_PER_STEP 0.225


void initializeTMC2209();
void rotateStepperMotor(float angle);
float convertAngle(float angle);
void calibrateStepperMotor(float angle);
void resetCurrentAngle();

extern float currentAngle;

#endif