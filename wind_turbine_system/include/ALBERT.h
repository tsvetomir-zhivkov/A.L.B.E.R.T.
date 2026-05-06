// Including a header guard
#ifndef ALBERT_H
#define ALBERT_H

#include <AS5600.h>

// I2C communication pins (AS5600 client)
#define AS5600_SDA_PIN 21
#define AS5600_SCL_PIN 22

#define AS5600_MAX_ANGLE 4095
#define AS5600_MIN_ANGLE 0

// Voltage sensor
#define R1 1
#define R2 1
#define ESP32_MAX_VOLTAGE 3.3
#define ESP32_MAX_ANALOG_VALUE 4095
#define VOLTAGE_SENSOR_MAX_VOLTAGE ESP32_MAX_VOLTAGE *((R1 + R2) / R2) 

// Error handling
#define READING_ERROR 10

// Define ESP32 GPIO pins


// @todo - are two variables better or memory allocation for error handling?
int reading_angle_min = 0;
int reading_angle_max = 0;

extern float AS5600_angle;

// Declare the functions used in ALBERT.cpp

void AS5600_connection_status(AS5600 &as5600);

bool AS5600_readAngle(AS5600 &as5600);

bool AS5600_validate_data(uint16_t raw_angle);

int isSwitchPressed(uint8_t pin);

void initializeStepperMotor();

void stopProcess();
#endif