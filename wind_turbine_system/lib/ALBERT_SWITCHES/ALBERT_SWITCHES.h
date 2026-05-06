#ifndef ALBERT_SWITCHES_H
#define ALBERT_SWITCHES_H

#define SWITCH_RIGHT_PIN 33
#define SWITCH_LEFT_PIN 32

#include <Arduino.h>

void initializeSwitches();
int isSwitchPressed(uint8_t pin);
 
#endif