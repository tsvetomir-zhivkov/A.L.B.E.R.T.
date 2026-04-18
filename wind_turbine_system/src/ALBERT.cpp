#include <Arduino.h>
#include "ALBERT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

// Initializing all required variables.

const char* ssid = "iPhone";
const char* password = "dZ3g-kJJ1-VRpa-7F8B";

AS5600 as5600;

// @todo - getting the id for the turbine by name for now, create the sensor, and create sensorLogs when turbineid and sensorid are fine

void setup() {
  
  Serial.begin(115200);

  // Initializes the Wire library and joins the I2C bus as a controller
  Wire.begin(AS5600_SDA_PIN, AS5600_SCL_PIN);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nConnected to the Wifi network");

  // Checks the connection status.
  AS5600_connection_status(as5600);

}

void loop() {
  
  // Create HTTPClient instance
  HTTPClient http;

  // serverPath
  String serverPath = "https://albert2026.azurewebsites.net/api/wind_turbines/2";

  // Initializing the connection between the server and the ESP32
  // c_str() returns a pointer to the same string null-terminated
  http.begin(serverPath.c_str());

  // Send HTTP request
  int httpResponseCode = http.GET();


  // Gets http response code
  if (httpResponseCode != 0)  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);

    JSONVar object = JSON.parse(payload);

    Serial.println(object["name"]);
  }
  else {
    Serial.print("Error");
  }

  // Free resources
  http.end();

  delay(10000);
  /*
  float wind_angle = AS5600_readAngle(as5600);
  Serial.print("ANGLE: ");
  Serial.println(wind_angle);
  delay(2000);
  */
}


// Verifies communication with the AS5600.
// Prints 'SUCCESS' if the magnetic encoder is detected.
// @param as5600 - Reference to an initialized AS5600 instance
void AS5600_connection_status(AS5600 &as5600) {

  int status = as5600.isConnected();
  if (status) {
    Serial.println("SUCCESS: AS5600 connected successfully");
  }
  else {
    Serial.println("FAIL: AS5600 was not detected");
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
    Serial.println("ERROR: The data provided by the magnetic encoder is not valid");
    return 0;
  }

}

// Validates that AS5600 works correctly
// @params angle - angle data provided by the magnetic encoder AS5600
// @return 0 - failure , 1 - sensor read data correctly
int AS5600_validate_data(uint16_t raw_angle) {
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