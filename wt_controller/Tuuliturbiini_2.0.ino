// When MS1 and MS2 are 0 => 1600 steps = 0.225 degrees per step (works in 1/8 mode for some reason)
// When MS1 and MS2 are 1 => 3200 steps = 0.1125 degrees per step (works in 1/16 mode)

// TODO: The button of the controller using millis() function

#include <Wire.h>
#include <AS5600.h>

#define E_pin 8
#define DIR_pin 10
#define STEP_pin 9
#define motor_indicator 7
#define manual_x A1
#define manual_b 2
#define braud 115200
#define steps_1_16 3200
#define steps_1_8 1600
#define delay_in_pulses_mks 1000
#define max_encoder_value 4095
#define error_steps 10
// that is the delay between the stepper motor rotations.
#define rotation_speed_ms 1000

// Full-rotation: 200 steps/rotation = 1.8 degrees per step
// May be useful to enable the microstepping (e.g. 1/16 mode)

AS5600 as5600;

// 0 - automatic
// 1 - manual
byte rotation_mode = 1; 

// Connecting the as5600 encoder
byte success = 0;

// Initializing values
double wind_angle_degrees;
int enable_p = LOW;
int direction_p = HIGH;
// Setting the current angle of the turbine at 0, which will be automatically changed when the as5600 is detected.
double current_turbine_angle = 0;

double angle_per_step = 360.0/steps_1_16;
double angle_to_add = angle_per_step;

// Angles for error handling
int min_angle = 0;
int max_angle = 0;

// Declaration of the functions
void rotateTurbine(int steps);
int AS5600_handler(double angle);
void change_rotation_mode(void);
void failure_handler(int failure_num);
void manual_rotation(void); 

void setup() {
  while(!Serial);
  
  Serial.begin(braud);
  Wire.begin();

  success = as5600.isConnected();
  if (success) {
    Serial.print("SUCCESS: AS5600 connected successfully");
    delay(1000);
  }
  else {
    Serial.print("ERROR: Connection failure");
  }

  // Initializing the stepper motor
  pinMode(E_pin, OUTPUT);
  pinMode(DIR_pin, OUTPUT);
  pinMode(STEP_pin, OUTPUT);
  pinMode(motor_indicator, OUTPUT);
  pinMode(manual_b, INPUT_PULLUP);

  // Interrupting the program when the rotation mode changes
  attachInterrupt(digitalPinToInterrupt(manual_b), change_rotation_mode , FALLING);
  
  // Enabling the tmc2209 driver
  digitalWrite(E_pin, enable_p);

  // Setting a direction of the stepper motor (CW by default)
  digitalWrite(DIR_pin, direction_p);
}

void loop() {
  if (success) {
    // automatic rotation mode
    if (rotation_mode == 0) {
      // Reading the angle of the wind (AS5600)
      double wind_angle = as5600.readAngle();
      
      // Mapping the results of the sensor
      wind_angle_degrees = (360*wind_angle)/max_encoder_value;
  
      // Checking if the magnetic encoder gets real data
      // Value indicating different failures (1,2,3,4,...)
      // 1 - AS5600 magnetic encoder does not read the data properly (reads only 0 or 360 for a long time)
      int failure = AS5600_handler(wind_angle_degrees);
      if (failure) {
        failure_handler(failure);     
        return;
      }
      
      Serial.println("Wind angle:");
      Serial.println("-------------");
      Serial.print("Raw data: ");
      Serial.print(wind_angle);
  
      Serial.print("\t");
      
      Serial.print("In degrees: ");
      Serial.println(wind_angle_degrees);
        
      // Calculating the difference between the current magnetic encoder position and current turbine position
      double offset = (wind_angle_degrees - current_turbine_angle);
      // if the angle is bigger than 180 degrees, approach it from the other direction
      if (abs(offset) > 180) {
        // examples: if 190:  (360 - 190) * -(190/190) = -180
        // if -270: (360 - 270)* -(-270/270) = 90
        offset = (360 - abs(offset))*(-(offset/abs(offset)));
      }
      // Now the offset is between -180 and 180 degrees
    
      // Calculating the steps the turbine should take
      double steps = offset / angle_per_step;
  
      Serial.print("STEPS:");
      Serial.print("\t");
      Serial.println(steps);
  
      // Rotating the turbine's head
      rotateTurbine((int) round(steps));
  
      Serial.println("\n");
      Serial.println("Turbine's position: ");
      Serial.println(current_turbine_angle);
      Serial.println("\n");
      
      delay(rotation_speed_ms); 
    }
   // manual rotation mode
   else {
      manual_rotation();      
   }
  }

  else {
    // Using exit function permanently, because it is not quite efficient. It stops the execution, but it does not free the cpu (while(1))
    exit(0);
  }
}

// Function that sends pulses to the stepper motor driver to control the stepper motor rotation. (automatic)
// param: steps - how many steps the stepper motor should take
void rotateTurbine(int steps) {

  if (steps >= 0) {
    // CW
    direction_p = HIGH;
    angle_to_add = angle_per_step;
  }
  else {
    // CCW
    direction_p = LOW;
    angle_to_add = -angle_per_step;
  }
  digitalWrite(DIR_pin, direction_p);

  // Transforming steps into a positive integer
  steps = abs(steps);
  
  // Sending pulses to the stepper motor (rotating the turbine)
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_pin, HIGH);
    delayMicroseconds(delay_in_pulses_mks);

    current_turbine_angle += angle_to_add;

    digitalWrite(STEP_pin, LOW);
    delayMicroseconds(delay_in_pulses_mks);
  }

}

// Function that checks if the magnetic encoder reads data correctly. 
// return: 0 if everything is fine and 1 if there is a failure.
int AS5600_handler(double angle) {
  if (min_angle >= error_steps || max_angle >= error_steps) {
    success = 0;
    return 1;
  }
  if (angle == 0.0) {
    min_angle++;
  }
  else if (angle == 360.0) {
    max_angle++;
  }
  else {
    min_angle = 0;
    max_angle = 0;
  }
  return 0;
}

// Function that handles the failures of the program
void failure_handler(int failure_num) {
  // Printing the failure message
  if (failure_num) {
    switch (failure_num) {
      case 1: 
        Serial.println("ERROR: AS5600 magnetic encoder does not read data properly");
        delay(1000);
        break;
      // TODO: add other cases
      default: 
        Serial.println("ERROR: Unknown");
        delay(1000);
        break;
    }
  }
  // Tell the program that there is a failure
  success = 0;
  // Turning off the tmc2209 driver
  enable_p = HIGH;
  digitalWrite(E_pin, enable_p);
  digitalWrite(motor_indicator, HIGH);
  return;
}

// Change the mode of the rotation (automatic vs. manual)
void change_rotation_mode(void) {
  // changing the output of the E_pin: if 0 => 0 == Low => 1, if 1 => 1 == Low => 0;
  enable_p = (enable_p == LOW);
  digitalWrite(E_pin, enable_p);
}

// Function for rotating the turbine's head manually, it is a different function for smoothness.
void manual_rotation(void) {

  // Sending pulses to the stepper motor (rotating the turbine)
  while (rotation_mode) {

    // Measure the movement of the controller
    int movement = map(analogRead(manual_x), 0, 1023, -512, 512);
    delayMicroseconds(delay_in_pulses_mks);
    Serial.print("Movement: ");
    Serial.print("\t");
    Serial.println(movement);

    // Using a tolerance of 100 to measure the movement, because the middle of the controller is not in the position (0,0).
    // Changing the direction of the movement
    // Because the directions of the controller are opposite of the directions of the motor, if movement is a positive then the steps will be negative, and vice versa.
    if (abs(movement) > 100) {
      if (movement < 0) {
        direction_p = HIGH;
        angle_to_add = angle_per_step;
      }
      else {
        direction_p = LOW;
        angle_to_add = -angle_per_step;
      }
      digitalWrite(DIR_pin, direction_p);

      // Sending pulses
      digitalWrite(STEP_pin, HIGH);
      delayMicroseconds(delay_in_pulses_mks);
  
      current_turbine_angle += angle_to_add;
  
      digitalWrite(STEP_pin, LOW);
      delayMicroseconds(delay_in_pulses_mks);
    }    
  }
}
