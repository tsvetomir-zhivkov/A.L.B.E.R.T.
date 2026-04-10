import postgres from "postgres";

// wt = wind turbine

// Explicity providing environment variables (including SSL settings) as an object to ensure a secure connection
// Thee connection to the Azure PostgreSQL database fails without this, because the function
// does not automatically read PGSSLMODE
const sql = postgres({
  host: Deno.env.get("PGHOST"),
  database:  Deno.env.get("PGDATABASE"),
  username: Deno.env.get("PGUSER"),
  password: Deno.env.get("PGPASSWORD"),
  port: Number(Deno.env.get("PGPORT")),
  ssl: Deno.env.get("PGSSLMODE"),
});

// Retrieves all wind turbines from the table
// Returns: all wind turbines currently stored in the table
export const readAllWindTurbines = async () => {
    return await sql 
    `SELECT * FROM wind_turbines;`;
}

// Retrieves a specific wind turbine from the table by id
// Parameters: an id of a wind turbine
// Returns: the wind turbine corresponding to the id
export const readOneWindTurbine = async (wtID) => {
    const res = await sql 
    `SELECT * FROM wind_turbines WHERE turbine_id = ${wtID};`;

    // postgres always returns a list including all relevant rows,
    // thus return only the first element of the list
    return res[0];
}

// Creates a new wind turbine and returns it if created successfully
// Parameters: an object containing the required data
// Returns: the created wind turbine
export const createWindTurbine = async (wt) => {
    const res = await sql 
    `INSERT INTO wind_turbines ${sql(wt)} RETURNING *;`;

    return res[0];
}

// Deletes a specific wind turbine from the table by id
// Parameters: an id of a wind turbine
// Returns: the deleted wind turbine or undefined if the row did not existed before deletion
export const deleteWindTurbine = async (wtID) => {
    const res = await sql 
    `DELETE FROM wind_turbines WHERE turbine_id = ${wtID} RETURNING *;`;

    return res[0];
}

// Updates a specific wind turbine from the table
// Parameters: an id of a wind turbine and an object containing updated data
// Returns: the updated wind turbine
export const updateWindTurbine = async (wtID, wt) => {

    // Checks whether new data is provided
    if (Object.keys(wt).length === 0) {
        // Updated only the changed_at parameter
        const res = await sql
        `UPDATE wind_turbines
        SET changed_at = NOW()
        WHERE turbine_id = ${wtID} RETURNING *;`;

        return res[0];
    }
    else {
        // Updates all parameters provided from the user
        const res = await sql
        `UPDATE wind_turbines
        SET ${sql(wt)}, changed_at = NOW()
        WHERE turbine_id = ${wtID} RETURNING *;`;

        return res[0];
    }
}