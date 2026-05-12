#ifndef ALBERT_SWITCHES_H
#define ALBERT_SWITCHES_H

#define SWITCH_RIGHT_PIN 32
#define SWITCH_LEFT_PIN 33

#include <Arduino.h>

void initializeSwitches();
int isSwitchPressed(uint8_t pin);
 
#endif