#include <Arduino.h>
#include "ALBERT.h"
#include "ALBERT_API.h"
#include "TMC2209.h"

// Initialize all required variables.
AS5600 as5600;
HTTPClient http;

// An error indicator (when success turns 0, terminate the process)
bool success = 1;

// parallel programming variables (non-blocking)
unsigned long lastMillisAS5600 = 0;
const unsigned long AS5600_read = 2000;
unsigned long lastMillisVoltage = 0;
const unsigned long voltage_read = 3000;

// Provided angle by the magnetic encoder AS5600
float AS5600_angle = 0;

void setup() {

  Serial.begin(115200);

  // Initialize the Wire library and joins the I2C bus as a controller
  Wire.begin(AS5600_SDA_PIN, AS5600_SCL_PIN);
  
  // Connect with the specific WiFi
  WiFi.begin(ssid, password);

  // Check if ESP32 is connected successfully to the provided WiFi
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConnected to the Wifi network\n");
  
  // Check the connection between wind turbine and serverAPI
  albert_connection_status(http);

  // Check the connection status.
  AS5600_connection_status(as5600);
  
  initializeAlbert();

}

void loop() {
  
  if (success) {

    // Send voltage data to the server
    if (millis() - lastMillisVoltage >= voltage_read) {
      lastMillisVoltage = millis();
      // Send generator and battery data to the server
      writeMeasurement(http, generatorV_id, readVoltage(VOLTAGE_SENSOR_PIN, VOLTAGE_SENSOR_MAX_VOLTAGE));
      writeMeasurement(http, battery_id, batteryCapacity(readVoltage(BATTERY_SENSOR_PIN, BATTERY_SENSOR_MAX_VOLTAGE)));
    }
    
    if (millis() - lastMillisAS5600 >= AS5600_read) {
      lastMillisAS5600 = millis();
      // Send AS5600 and stepper motor data to the server
      AS5600_readAngle(as5600);
      writeMeasurement(http, as5600_id, AS5600_angle);
      writeMeasurement(http, stepper_motor_id, currentAngle);
    }

    rotateStepperMotor(AS5600_angle);
    
  } 
}

// Verifies communication with the AS5600.
// Prints 'SUCCESS' if the magnetic encoder is detected.
// @param as5600 - Reference to an initialized AS5600 instance
void AS5600_connection_status(AS5600 &as5600) {

  int status = as5600.isConnected();
  if (status) {
    Serial.println("\nSUCCESS: AS5600 connected successfully");
  }
  else {
    Serial.println("\nFAIL: AS5600 was not detected");
  }

}

// Reads angle from AS5600 magnetic encoder; reads data every 3 seconds
// @param as5600 - Reference to an initialized AS5600 instance
// @return 0-failure, 1-success
bool AS5600_readAngle(AS5600 &as5600) {

  // Reads the data provided by the magnetic encoder AS5600.
  uint16_t wind_angle = as5600.readAngle();

  /*
  // Validate that AS5600 encoder is working correctly
  if (!AS5600_validate_data(wind_angle)) {
    stopProcess();
  }
  */
  // Maps the data provided by the magnetic encoder AS5600 (raw data -> angle).
  float wind_angle_degrees = wind_angle * AS5600_RAW_TO_DEGREES;

  // Checks whether the data provided by the magnetic encoder is valid.
  if (wind_angle_degrees <= 360.0 & wind_angle_degrees >= 0.0) {

    Serial.print("\nWind angle: ");
    Serial.println(wind_angle_degrees);
    AS5600_angle = wind_angle_degrees;

    Serial.print("\nStepper Motor angle: ");
    Serial.println(currentAngle);
    return 1;
  }
  else {
    Serial.println("\nERROR: The data provided by the magnetic encoder is not valid");
    return 0;
  }
}

// Validates that AS5600 works correctly
// @params angle - angle data provided by the magnetic encoder AS5600
// @return 0 - failure , 1 - sensor read data correctly
bool AS5600_validate_data(uint16_t raw_angle) {
  // If AS5600 returns only 0s or 4096s for t consecutive readings.
  if (reading_angle_max >= READING_ERROR || reading_angle_min >= READING_ERROR){
    return 0;
  }
  // If AS5600 returns only 4096s
  if (raw_angle == AS5600_MAX_ANGLE) {
    reading_angle_max++;
  }
  // If AS5600 returns only 0s
  else if (raw_angle == AS5600_MIN_ANGLE) {
    reading_angle_min++;
  }
  else {
    reading_angle_max = 0;
    reading_angle_min = 0;
  }
  return 1;
}


void stopProcess() {
  Serial.println("Program executed");
  exit(0);
}



////////////////////////////////////////////
//            VOLTAGE SENSOR              //
////////////////////////////////////////////

float readVoltage(uint8_t pin, float maxVoltage) {

  // Read analog data from the provided pin
  uint16_t data = analogRead(pin);

  // Map the analog input to a voltage value
  float voltage = (data/ESP32_MAX_ANALOG_VALUE) * maxVoltage;
  
  // Return the result + error value
  return voltage;
}

int batteryCapacity(float voltage) {
  return (voltage/BATTERY_CAPACITY)*100;
}


////////////////////////////////////////////
//            INITIALIZATION              //
////////////////////////////////////////////

void initializeAlbert() {
  
  initializeSwitches();
  initializeTMC2209();
  initializeStepperMotor();

  // Initialize the voltage sensor
  pinMode(VOLTAGE_SENSOR_PIN, INPUT);
  pinMode(BATTERY_SENSOR_PIN, INPUT);
}
