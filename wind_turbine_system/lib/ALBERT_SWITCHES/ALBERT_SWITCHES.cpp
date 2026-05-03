#include "ALBERT_SWITCHES.h";

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
