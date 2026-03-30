// Imports the repository to communicate with the database
import * as sensorRepository from "../repositories/sensorRepository.js";

export const readAll = async (c) => {
    // Uses the given parameter in the url
    const wtID = Number(await c.req.param("wind_turbineID"));

    // Checks whether sensorID is a valid number
    if (Number.isNaN(wtID)) {
        // Returns code 400 - bad request
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    // Fetches the sensors from the table
    const res = await sensorRepository.readAllSensors(wtID);

    // If res is undefined, return code 404 - not found
    if (!res) {
        return c.json({message: "Wind turbine not found"}, 404)
    }

    if (res.length === 0) {
        return c.json({message: "No sensors found"}, 200);
    }

    // Returns the required sensors if found
    return c.json(res, 200);
}

export const readOne = async (c) => {
    // Uses the given parameters in the url
    const wtID = Number(await c.req.param("wind_turbineID"));
    const sensorID = Number(await c.req.param("sensorID"));

    // Checks whether wtID or sensorID is a valid number
    if (Number.isNaN(wtID)) {
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    if (Number.isNaN(sensorID)) {
        return c.json({message: "Invalid sensor id"}, 400);
    }

    // Fetches one sensor from the table
    const res = await sensorRepository.readOneSensor(wtID, sensorID);

    // If res is undefined, return code 404 - not found
    if (!res) {
        return c.json({message: "Sensor not found"}, 404);
    }

    return c.json(res, 200);
}

export const createOne = async (c) => {
    // Uses the given parameter in the url
    const wtID = Number(await c.req.param("wind_turbineID"));

    // Checks whether sensorID is a valid number
    if (Number.isNaN(wtID)) {
        // Returns code 400 - bad request
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    // Uses the provided data from the user
    const sensor = await c.req.json();

    if (!sensor.unit) {
        return c.json({message: "Missing sensor data"}, 400);
    }

    sensor.turbine_id = wtID;

    const res = await sensorRepository.createOneSensor(sensor);

    if (!res) {
        return c.json({message: "An error occurred"}, 500);
    }

    return c.json(res, 200);
}

export const deleteOne = async (c) => {
    // Uses the given parameters in the url
    const wtID = Number(await c.req.param("wind_turbineID"));
    const sensorID = Number(await c.req.param("sensorID"));

    // Checks whether wtID or sensorID is a valid number
    if (Number.isNaN(wtID)) {
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    if (Number.isNaN(sensorID)) {
        return c.json({message: "Invalid sensor id"}, 400);
    }

    // Deletes the specific sensor from the table and returns it
    const res = await sensorRepository.deleteOneSensor(wtID, sensorID);

    if (!res) {
        return c.json({message: "Sensor not found"}, 404);
    }

    return c.json(res, 200);
}

export const updateOne = async (c) => {
    // Uses the given parameters in the url
    const wtID = Number(await c.req.param("wind_turbineID"));
    const sensorID = Number(await c.req.param("sensorID"));

    // Checks whether wtID or sensorID is a valid number
    if (Number.isNaN(wtID)) {
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    if (Number.isNaN(sensorID)) {
        return c.json({message: "Invalid sensor id"}, 400);
    }

    // Checks the content given in the request
    const header = await c.req.header("Content-Type");

    // Use the json content if provided
    let sensor = {};
    if (header == "application/json") {
        // Uses the given data from the json object/file
        sensor = await c.req.json();
    }

    sensor.turbine_id = wtID;

    // Updates the specific sensor and returns it
    const res = await sensorRepository.updateSensor(wtID, sensorID, sensor);

    if (!res) {
        return c.json({message: "Sensor not found"}, 404);
    }

    return c.json(res, 200);
}