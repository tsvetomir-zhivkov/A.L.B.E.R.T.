#include "ALBERT_SWITCHES.h"

// Initialize the switches (rotor's moving constraints)
void initializeSwitches() {
  pinMode(SWITCH_LEFT_PIN, INPUT_PULLUP);
  pinMode(SWITCH_RIGHT_PIN, INPUT_PULLUP);
}

// Check whether a switch is pressed
// @param pin switch's pin
// @return 0-not pressed, 1-pressed
int isSwitchPressed(uint8_t pin) {

  // Read switch data
  int readSwitch = digitalRead(pin);

  // If a switch is pressed, return 1;
  if (readSwitch == LOW) {
    return 1;
  }
  return 0;
  
}
