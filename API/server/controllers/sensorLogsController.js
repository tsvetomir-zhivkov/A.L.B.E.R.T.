// Imports the repository to communicate with the database
import * as sensorLogsRepository from "../repositories/sensorLogsRepository.js";

export const readAll = async (c) => {
    // Fetches the sensors from the table
    const res = await sensorLogsRepository.readAllSensorLogs();

    if (res.length === 0) {
        return c.json({message: "No sensors' logs found"}, 200);
    }

    return c.json(res, 200);
}

export const readAllLast = async (c) => {
    // Fetches the sensors from the table
    const res = await sensorLogsRepository.readAllSensorLogsLast();

    if (res.length === 0) {
        return c.json({message: "No sensors' logs found"}, 200);
    }

    return c.json(res, 200);
}

export const readOne = async (c) => {
    // Uses the given parameters in the url
    const wtID = Number(await c.req.param("wind_turbineID"));
    const sensorID = Number(await c.req.param("sensorID"));

    // Checks whether wtID and sensorID is a valid number
    if (Number.isNaN(wtID)) {
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    if (Number.isNaN(sensorID)) {
        return c.json({message: "Invalid sensor id"}, 400);
    }

    // Fetches one sensor from the table
    const res = await sensorLogsRepository.readOneSensorLastLog(wtID, sensorID);

    // If res is undefined, return code 404 - not found
    if (!res) {
        return c.json({message: "Sensor's logs not found"}, 404);
    }

    return c.json(res, 200);
}

export const createOne = async (c) => {
    // Uses the given parameter in the url
    const wtID = Number(await c.req.param("wind_turbineID"));
    const sensorID = Number(await c.req.param("sensorID"));

    // Checks whether wtID and sensorID is a valid number
    if (Number.isNaN(wtID)) {
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    if (Number.isNaN(sensorID)) {
        // Returns code 400 - bad request
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    // Uses the provided data from the user
    const sensor = await c.req.json();

    if (!sensor.measurement) {
        return c.json({message: "Missing sensor data"}, 400);
    }

    sensor.sensor_id = sensorID;
    sensor.turbine_id = wtID;

    const res = await sensorLogsRepository.createOneSensor(sensor);

    if (!res) {
        return c.json({message: "An error occurred"}, 500);
    }

    return c.json(res, 200);
}

export const deleteOne = async (c) => {
    // Uses the given parameters in the url
    const wtID = Number(await c.req.param("wind_turbineID"));
    const sensorID = Number(await c.req.param("sensorID"));

    // Checks whether wtID and sensorID is a valid number
    if (Number.isNaN(wtID)) {
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    if (Number.isNaN(sensorID)) {
        return c.json({message: "Invalid sensor id"}, 400);
    }

    // Deletes the specific sensor's logs from the table and returns it
    const res = await sensorLogsRepository.deleteOneSensor(wtID, sensorID);

    if (!res) {
        return c.json({message: "Sensor's logs not found"}, 404);
    }

    return c.json(res, 200);
}
