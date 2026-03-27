import postgres from "postgres";

// wt = wind turbine

// Postgres will automatically read the environment 
// variables needed for it to operate correctly.
const sql = postgres();

// Retrieves all rows from the table
// Returns: all rows currently stored in the table
export const readAllWindTurbines = async () => {
    return await sql 
    `SELECT * FROM wind_turbines;`;
}

// Retrieves a specific row from the table by id
// Parameters: an id of a row
// Returns: the row corresponding to the id
export const readOneWindTurbine = async (wtID) => {
    const res = await sql 
    `SELECT * FROM wind_turbines WHERE turbine_id = ${wtID};`

    // postgres always returns a list including all relevant rows,
    // thus return only the first element of the list
    return res[0];
}

// Creates a new row and returns it if created successfully.
// Parameters: an object containing the required data.
// Returns: the created row
export const createWindTurbine = async (wt) => {
    const res = await sql 
    `INSERT INTO wind_turbines (name, height, rotor_diameter, weight)
     VALUES (${wt.name}, ${wt.height}, ${wt.rotor_diameter}, ${wt.weight}) RETURNING *;`;

    return res[0];
}

// Deletes a specific row from the table by id
// Parameters: an id of a row
// Returns: the deleted row or undefined if the row did 
// not existed before deletion.
export const deleteWindTurbine = async (wtID) => {
    const res = await sql 
    `DELETE FROM wind_turbines WHERE turbine_id = ${wtID} RETURNING *;`;

    return res[0];
}

// Updates a specific row from the table
// Parameters: an id of a row, which is required, and an object containing the new data
// Returns: the updated row
export const updateWindTurbine = async (wtID, wt) => {

    // Checks if new data is not given
    if (Object.keys(wt).length === 0) {
        // Updated only the changed_at parameter
        const res = await sql
        `UPDATE wind_turbines
        SET changed_at = NOW()
        WHERE turbine_id = ${wtID} RETURNING *;`;

        return res[0];
    }
    else {
        // Updates all parameters given in the object wt
        const res = await sql
        `UPDATE wind_turbines
        SET ${sql(wt)}, changed_at = NOW()
        WHERE turbine_id = ${wtID} RETURNING *;`;

        return res[0];
    }
}