import {postgres} from "postgres";

// Postgres will automatically read the environment variables
// needed for it to operate correctly
const sql = postgres();

export const readAll = async () => {
    return await sql 
    'SELECT * FROM wind_turbines';
}