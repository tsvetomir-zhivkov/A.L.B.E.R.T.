#include <Arduino.h>
#include "ALBERT.h"
#include "ALBERT_API.h"

// Initializing all required variables.

AS5600 as5600;

HTTPClient http;

// An error indicator
bool success = 1;

// parallel programming (non-blocking)
unsigned long lastMillis = 0;
const unsigned long AS5600_read = 2000;

// @todo - getting the id for the turbine by name for now, create the sensor, and create sensorLogs when turbineid and sensorid are fine

void setup() {
  
  Serial.begin(115200);

  // Initialize the Wire library and joins the I2C bus as a controller
  Wire.begin(AS5600_SDA_PIN, AS5600_SCL_PIN);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
  }

  Serial.println("\nConnected to the Wifi network");

  // Check the connection status.
  AS5600_connection_status(as5600);

  // Check the connection between wind turbine and serverAPI
  albert_connection_status(http);


}

void loop() {

  if (success) {

    if (millis() - lastMillis >= AS5600_read) {

      lastMillis = millis();
      float wind_angle = AS5600_readAngle(as5600);
      Serial.print("ANGLE: ");
      Serial.println(wind_angle);
      //success = writeMeasurement(http, as5600_id, wind_angle);

    }
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

// Reads angle from AS5600 magnetic encoder.
// @param as5600 - Reference to an initialized AS5600 instance
// @return the angle measured by the AS5600 encoder
float AS5600_readAngle(AS5600 &as5600) {

  // Reads the data provided by the magnetic encoder AS5600.
  uint16_t wind_angle = as5600.readAngle();

  // Validate that AS5600 encoder is working correctly
  if (!AS5600_validate_data(wind_angle)) {
    stopProcess();
  }

  // Maps the data provided by the magnetic encoder AS5600 (raw data -> angle).
  float wind_angle_degrees = wind_angle * AS5600_RAW_TO_DEGREES;

  // Checks whether the data provided by the magnetic encoder is valid.
  if (wind_angle_degrees <= 360.0 & wind_angle_degrees >= 0.0) {
    return wind_angle_degrees;
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
}