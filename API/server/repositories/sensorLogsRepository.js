import postgres from "postgres";

const sql = postgres({
  host: Deno.env.get("PGHOST"),
  database:  Deno.env.get("PGDATABASE"),
  username: Deno.env.get("PGUSER"),
  password: Deno.env.get("PGPASSWORD"),
  port: Number(Deno.env.get("PGPORT")),
  ssl: Deno.env.get("PGSSLMODE"),
});

// Retrieves all sensors' logs
// Returns: all sensors's logs stored in the table
export const readAllSensorLogs = async () => {
    return await sql
    `SELECT * FROM sensor_logs;`;
}

// Retrieves all the latest sensor logs
// Returns: all the latest sensor logs stored in the table
export const readAllSensorLogsLast = async () => {
    return await sql
    `SELECT DISTINCT ON (sensor_id) sensor_id, measurement, created_at
     FROM sensor_logs
     ORDER BY sensor_id, created_at DESC;`;
}

// Retrieves a specific sensor's last log from the table
// Parameters: a sensor id
// Returns: the sensor's measurement corresponding to the sensor id
export const readOneSensorLastLog = async (wtID, sensorID) => {
    const res = await sql
    `SELECT DISTINCT ON (sensor_id) sensor_id, measurement, created_at
     FROM sensor_logs
     WHERE sensor_id = ${sensorID} AND turbine_id = ${wtID}
     ORDER BY sensor_id, created_at DESC;`;

    return res[0];
}

// Creates a new sensor's log and returns it if created successfully
// Parameters: an object containing the required data
// Returns: the created sensor's log
export const createOneSensor = async (sensor) => {
    const res = await sql
    `INSERT INTO sensor_logs ${sql(sensor)} RETURNING *;`;

    return res[0];
}

// Deletes a specific sensor's log from the table
// Parameters: an id of a sensor
// Returns: the deleted sensor's log or undefined if the row did not existed before deletion
export const deleteOneSensor = async (wtID, sensorID) => {
    const res = await sql
    `DELETE FROM sensor_logs WHERE sensor_id = ${sensorID} AND turbine_id = ${wtID} RETURNING *;`;

    return res[0];
}
