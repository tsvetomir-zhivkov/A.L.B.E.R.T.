import postgres from "postgres";

const sql = postgres({
  host: Deno.env.get("PGHOST"),
  database:  Deno.env.get("PGDATABASE"),
  username: Deno.env.get("PGUSER"),
  password: Deno.env.get("PGPASSWORD"),
  port: Number(Deno.env.get("PGPORT")),
  ssl: Deno.env.get("PGSSLMODE"),
});

// Retrieves all sensors corresponding to a turbine id
// Returns: all sensors stored in the table corresponding to a turbine id
export const readAllSensors = async (turbineID) => {
    return await sql
    `SELECT * FROM sensors WHERE turbine_id = ${turbineID};`;
}

// Retrieves a specific sensor from the table by turbine and sensor id
// Parameters: turbine and sensor ids
// Returns: the sensor corresponding to the ids
export const readOneSensor = async (turbineID, sensorID) => {
    const res = await sql
    `SELECT * FROM sensors WHERE turbine_id = ${turbineID} AND sensor_id = ${sensorID};`;

    return res[0];
}

// Creates a new sensor and returns it if created successfully
// Parameters: an object containing the required data
// Returns: the created sensor
export const createOneSensor = async (sensor) => {
    // Dynamical insert
    const res = await sql
    `INSERT INTO sensors ${sql(sensor)} RETURNING *;`;

    return res[0];
}

// Deletes a specific sensor from the table by id
// Parameters: a wind turbine id and sensor id
// Returns: the deleted sensor or undefined if the row did not existed before deletion
export const deleteOneSensor = async (turbineID, sensorID) => {
    const res = await sql
    `DELETE FROM sensors WHERE sensor_id = ${sensorID} AND turbine_id = ${turbineID} RETURNING *;`;

    return res[0];
}

// Updated the data of a specific sensor in the table
// Parameters: an id of a sensor and an object containing updated sensor data
// Returns: the updated row
export const updateSensor = async (turbineID, sensorID, sensor) => {

    // Updates all parameters provided from the user (turbine_id is always provided)
    const res = await sql
    `UPDATE sensors SET ${sql(sensor)}, changed_at = NOW()
    WHERE sensor_id = ${sensorID} AND turbine_id = ${turbineID} RETURNING *;`;
  
    return res[0];
}