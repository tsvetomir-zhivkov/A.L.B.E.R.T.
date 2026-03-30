import postgres from "postgres";

const sql = postgres();

// Retrieves all wind turbines' connection information from the database
// Returns: the connection data
export const readAllConnections = async () => {
    return await sql 
    `SELECT * FROM turbine_connection;`;
}

// Retrieves a specific wind turbine's connection information from the table
// Parameters: an id of a wind turbine
// Returns: the wind turbine's connection information
export const readOneConnection = async (wtID) => {
    const res = await sql 
    `SELECT * FROM turbine_connection WHERE turbine_id = ${wtID};`;

    return res[0];
}

// Creates a new wind turbine's connection information and returns it if created successfully
// Parameters: an object containing the required data
// Returns: the created wind turbine's connection information
export const createConnection = async (wt) => {
    const res = await sql 
    `INSERT INTO turbine_connection ${sql(wt)} RETURNING *;`;

    return res[0];
}

// Deletes a specific wind turbine's connection information from the table
// Parameters: an id of a wind turbine
// Returns: the deleted wind turbine's connection information or undefined if the row did not existed before deletion
export const deleteConnection = async (wtID) => {
    const res = await sql 
    `DELETE FROM turbine_connection WHERE turbine_id = ${wtID} RETURNING *;`;

    return res[0];
}

// Updates a specific wind turbine's connection information from the table
// Parameters: an id of a wind turbine and an object containing updated data
// Returns: the updated wind turbine's connection information
export const updateConnection = async (wtID, connection) => {

    // Checks whether new data is provided
    if (Object.keys(connection).length === 0) {
        // Updated only the changed_at parameter
        const res = await sql
        `UPDATE turbine_connection
        SET changed_at = NOW()
        WHERE turbine_id = ${wtID} RETURNING *;`;

        return res[0];
    }
    else {
        // Updates all parameters provided from the user
        const res = await sql
        `UPDATE turbine_connection
        SET ${sql(connection)}, changed_at = NOW()
        WHERE turbine_id = ${wtID} RETURNING *;`;

        return res[0];
    }
}