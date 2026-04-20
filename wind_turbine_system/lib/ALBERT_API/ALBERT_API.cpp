#include "ALBERT_API.h"

const char* ssid = "iPhone";
const char* password = "dZ3g-kJJ1-VRpa-7F8B";

String serverPath = "https://albert2026.azurewebsites.net/api";

uint16_t albert_id = 4;
uint16_t as5600_id = 1;

bool albert_connection_status(HTTPClient &http) {

    // Ensure the wind turbine exists in the database
    bool verification_status = verifyWindTurbine(http);

    if (!verification_status) {
        Serial.println("Verification error");
        return 0;
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

    Serial.print("Verifying the wind turbine with ID ");
    Serial.println(albert_id);

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

    Serial.println("Creating a wind turbine");

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
