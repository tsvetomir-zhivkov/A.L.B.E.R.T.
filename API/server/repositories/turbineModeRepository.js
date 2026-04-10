import postgres from "postgres";

const sql = postgres({
  host: Deno.env.get("PGHOST"),
  database:  Deno.env.get("PGDATABASE"),
  username: Deno.env.get("PGUSER"),
  password: Deno.env.get("PGPASSWORD"),
  port: Number(Deno.env.get("PGPORT")),
  ssl: Deno.env.get("PGSSLMODE"),
});

// Retrieves all wind turbines along with their working modes
// Returns: all wind turbines along with their working modes
export const readAllWindTurbineModes = async () => {
    return await sql 
    `SELECT * FROM turbine_mode;`;
}

// Retrieves a specific wind turbine along with its working mode from the table
// Parameters: an id of a wind turbine
// Returns: the working mode of the requested wind turbine
export const readOneWindTurbineMode = async (wtID) => {
    const res = await sql 
    `SELECT * FROM turbine_mode WHERE turbine_id = ${wtID};`;

    return res[0];
}

// Creates a working mode of a specific wind turbine and returns it if created successfully
// Parameters: a wind turbine's id and its working mode (either "auto" or "manual")
// Returns: the created wind turbine's working mode
export const createWindTurbineMode = async (wtID, mode) => {
    const res = await sql 
    `INSERT INTO turbine_mode (turbine_id, mode) VALUES (${wtID}, ${mode}) RETURNING *;`;

    return res[0];
}

// Deletes the working mode of a specific wind turbine from the table
// Parameters: an id of a wind turbine
// Returns: the deleted wind turbine's working mode or undefined if the row did not existed before deletion
export const deleteWindTurbineMode = async (wtID) => {
    const res = await sql 
    `DELETE FROM turbine_mode WHERE turbine_id = ${wtID} RETURNING *;`;

    return res[0];
}

// Updates a specific wind turbine's working mode in the database table
// Parameters: an id of a wind turbine and the updated working mode
// Returns: the updated wind turbine's working mode
export const updateWindTurbine = async (wtID, mode) => {

    // Updates the working mode of a specific wind turbine
    const res = await sql
    `UPDATE turbine_mode
    SET mode = ${mode}, changed_at = NOW()
    WHERE turbine_id = ${wtID} RETURNING *;`;

    return res[0];
}