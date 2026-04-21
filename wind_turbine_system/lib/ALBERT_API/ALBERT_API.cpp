#include "ALBERT_API.h"

const char* ssid = "iPhone";
const char* password = "dZ3g-kJJ1-VRpa-7F8B";

String serverPath = "https://albert2026.azurewebsites.net/api";

// Wind turbines
uint16_t albert_id = 4;

// Sensors
uint16_t as5600_id = 1;
uint16_t stepper_motor_id = 2;

bool albert_connection_status(HTTPClient &http) {

    // Ensure the wind turbine exists in the database
    bool verification_status_wt = verifyWindTurbine(http);

    if (!verification_status_wt) {
        Serial.println("\nVerification error (A.L.B.E.R.T.)");
        return 0;
    }

    Serial.println("\n................................");

    // Ensure the sensors exist in the database
    bool verification_status_as5600 = verifySensor(http, as5600_id);

    if (!verification_status_as5600) {
        Serial.println("\nVerification error (AS5600)");
    }

    bool verification_status_sm = verifySensor(http, stepper_motor_id);

    if (!verification_status_sm) {
        Serial.println("\nVerification error (stepper_motor)");
    }

    //@todo Fix server mode endpoint and database
    Serial.println("Connection successful");
    
    return 1;

}

// Check if a wind turbine with the specified ID exists; if not, create it.
// @param http - HTTPClient instance
// @return 1 - success, 0 - failure
bool verifyWindTurbine(HTTPClient &http) {

    // Fetch wind turbine data provided by the specific server endpoint.
    String turbineID_path = serverPath + "/wind_turbines/" + String(albert_id);

    // Initialize the connection between serverAPI and ESP32
    http.begin(turbineID_path.c_str());

    // Send HTTP request
    int httpResponseCode = http.GET();

    Serial.print("\nEnsuring the wind turbine with ID ");
    Serial.print(albert_id);
    Serial.println("exists...");

    if (httpResponseCode == 200) {

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        return 1;

    }
    else if (httpResponseCode == 404) {

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.println("Wind turbine not found");

        // If a wind turbine with the specified ID is not found, create it.
        bool creation_status = createWindTurbine(http);

        if (!creation_status) {
            return 0;
        }

        return 1;
    }
    else {

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.println("Server error");
        
        return 0;
    }
}

// Add a wind turbine to the database; create an entry
// @param http - HTTPClient instance
// @return 1 - success, 0 - failure
bool createWindTurbine(HTTPClient &http) {

    // Server endpoint for creating wind turbine instances
    String creationPath = serverPath + "/wind_turbines";

    // Initialize connection
    http.begin(creationPath.c_str());

    // Add the required headers
    http.addHeader("Content-Type", "application/json");

    // Create the JSON object that will be part of the request.
    JSONVar data;
    data["name"] = ALBERT_NAME;
    data["height"] = ALBERT_HEIGHT;
    data["rotor_diameter"] = ALBERT_ROTOR_DIAMETER;
    data["weight"] = ALBERT_WEIGHT;

    // Stringify the JSON object
    String httpRequestData = JSON.stringify(data);

    // Send HTTP request
    int httpResponseCode = http.POST(httpRequestData);

    Serial.println("\nCreating a wind turbine...");

    // @todo Change response code to 201
    if (httpResponseCode == 200) {

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        JSONVar data = JSON.parse(http.getString());

        // Update the wind turbine ID
        albert_id = data["turbine_id"];

        return 1;

    }
    else {

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.println("Server error");

        return 0;
    }

}


// Check if a sensor with the specified ID exists; if not, create it.
// @param http - HTTPClient instance and sensor ID
// @return 1 - success, 0 - failure
bool verifySensor(HTTPClient &http, uint16_t &sensorID) {

    // Fetch wind turbine data provided by the specific server endpoint.
    String sensorID_path = serverPath + "/wind_turbines/" + String(albert_id) + "/sensors/" + String(sensorID);

    // Initialize the connection between serverAPI and ESP32
    http.begin(sensorID_path.c_str());

    // Send HTTP request
    int httpResponseCode = http.GET();

    Serial.print("\nEnsuring the sensor with ID ");
    Serial.print(sensorID);
    Serial.println(" exists...");

    if (httpResponseCode == 200) {

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        return 1;

    }
    else if (httpResponseCode == 404) {

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.println("Sensor not found");

        // If a wind turbine with the specified ID is not found, create it.
        bool creation_status = createSensor(http, sensorID);

        if (!creation_status) {
            return 0;
        }

        return 1;
    }
    else {

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.println("Server error");
        
        return 0;
    }

}

// Add a sensor to the database; create an entry
// @param http - HTTPClient instance and sensor ID
// @return 1 - success, 0 - failure
bool createSensor(HTTPClient &http, uint16_t &sensorID) {

    // DATABASE: sensors

    // Server endpoint for creating wind turbine instances
    String creationPath = serverPath + "/wind_turbines/" + String(albert_id) + "/sensors";

    // Initialize connection
    http.begin(creationPath.c_str());

    // Add the required headers
    http.addHeader("Content-Type", "application/json");

    // Create the JSON object that will be part of the request.
    JSONVar data;
    data["turbine_id"] = albert_id;
    data["name"] = AS5600_NAME;
    data["unit"] = AS5600_UNIT;

    // Stringify the JSON object
    String httpRequestData = JSON.stringify(data);

    // Send HTTP request
    int httpResponseCode = http.POST(httpRequestData);

    Serial.println("\nCreating a sensor...");

    // @todo Change response code to 201
    if (httpResponseCode == 200) {

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        JSONVar data = JSON.parse(http.getString());

        // Update the wind turbine ID
        sensorID = data["sensor_id"];

        return 1;

    }
    else {

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.println("Server error");

        return 0;
    }

}

// Insert a sensor log to the database
// @param http - HTTPClient instance, sensor ID and a sensor measurement
// @return 1 - success, 0 - failure
bool writeMeasurement(HTTPClient &http, uint16_t sensorID, float sensor_measurement) {

    // Server endpoint for creating wind turbine instances
    String creationPath = serverPath + "/wind_turbines/" + String(albert_id) + "/sensors/" + String(sensorID) + "/logs";

    // Initialize connection
    http.begin(creationPath.c_str());

    // Add the required headers
    http.addHeader("Content-Type", "application/json");

    // Create the JSON object that will be part of the request.
    JSONVar data;
    data["sensor_id"] = sensorID;
    data["turbine_id"] = albert_id;
    data["measurement"] = sensor_measurement;

    // Stringify the JSON object
    String httpRequestData = JSON.stringify(data);

    // Send HTTP request
    int httpResponseCode = http.POST(httpRequestData);

    Serial.println("\nInserting a sensor log...");

    // @todo Change response code to 201
    if (httpResponseCode == 200) {

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        return 1;

    }
    else {

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.println("Server error");

        return 0;
    }

}