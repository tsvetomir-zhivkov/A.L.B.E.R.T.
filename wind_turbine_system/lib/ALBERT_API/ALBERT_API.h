#ifndef ALBERT_API_H
#define ALBERT_API_H

#include <HTTPClient.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

// A.L.B.E.R.T. parameters
#define ALBERT_NAME "name"
#define ALBERT_HEIGHT 20
#define ALBERT_ROTOR_DIAMETER 20
#define ALBERT_WEIGHT 20

// AS5600 parameters
#define AS5600_NAME "AS5600"
#define AS5600_UNIT "degrees"

// WiFi credentials
extern const char* ssid;
extern const char* password;

// Server domain as a string for easier concatenation
extern String serverPath;

// Unique wind turbine and sensor IDs
extern uint16_t albert_id;
extern uint16_t as5600_id;


bool albert_connection_status(HTTPClient &http);
bool verifyWindTurbine(HTTPClient &http);
bool createWindTurbine(HTTPClient &http);
bool verifySensor(HTTPClient &http, uint16_t &sensorID);
bool createSensor(HTTPClient &http, uint16_t &sensorID);
bool writeMeasurement(HTTPClient &http, uint16_t sensorID, float sensor_measurement);


#endif