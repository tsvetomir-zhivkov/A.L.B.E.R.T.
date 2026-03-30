// Imports the repository to communicate with the database
import * as connectionRepository from "../repositories/connectionRepository.js";

// Using universal function names for ease when creating the handlers.

export const readAll = async (c) => {
    // Fetches all rows from the table
    const res = await connectionRepository.readAllConnections();

    // If there are not rows in the table, return code 200
    if (res.length === 0) {
        return c.json({message: "No connection information found for the specific wind turbine"}, 200);
    }

    return c.json(res, 200);
}

export const readOne = async (c) => {
    // Uses the given parameter in the url
    const wtID = Number(await c.req.param("wind_turbineID"));

    // Checks whether wtID is a valid number
    if (Number.isNaN(wtID)) {
        // Returns code 400 - bad request   
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    // Fetches one row from the table
    const res = await connectionRepository.readOneConnection(wtID);

    // If res is undefined, return code 404 - not found
    if (!res) {
        return c.json({message: "Wind turbine's connection information not found"}, 404);
    }

    // Returns the required wind turbine if found
    return c.json(res, 200);
}

export const createOne = async (c) => {
    // Uses the parameters provided in the url
    const wtID = Number(await c.req.param("wind_turbineID"));

    if (Number.isNaN(wtID)) {
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    // Checks the content given in the request
    const header = await c.req.header("Content-Type");

    // Use the json content if provided
    let wt = {};
    if (header == "application/json") {
        // Uses the given data from the json object/file
        wt = await c.req.json();
    }

    wt.turbine_id = wtID;

    const res = await connectionRepository.createConnection(wt);

    // Checks if an error is found
    if (!res) {
        // Returns code 500 - internal server error
        return c.json({message: "An error occurred"}, 500);
    }

    return c.json(res, 200);
}

export const deleteOne = async (c) => {
    // Uses the given parameter in the url
    const wtID = Number(await c.req.param("wind_turbineID"));

    // Checks whether wtID is a valid number
    if (Number.isNaN(wtID)) {
        // Returns code 400 - bad request   
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    // Deletes the specific row and returns it
    const res = await connectionRepository.deleteConnection(wtID);

    // Checks if the row existed before deletion
    if (!res) {
        return c.json({message: "Wind turbine's connection information not found"}, 404);
    }

    return c.json(res, 200);
}

export const updateOne = async (c) => {
    // Uses the given parameter in the url
    const wtID = Number(await c.req.param("wind_turbineID"));

    // Checks whether wtID is a valid number
    if (Number.isNaN(wtID)) {
        // Returns code 400 - bad request   
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    // Checks the content given in the request
    const header = await c.req.header("Content-Type");

    // Use the json content if provided
    let wt = {};
    if (header == "application/json") {
        // Uses the given data from the json object/file
        wt = await c.req.json();
    }
    // Updates the specific row and returns it
    const res = await connectionRepository.updateConnection(wtID, wt);

    if (!res) {
        return c.json({message: "Wind turbine's connection information not found"}, 404);
    }

    return c.json(res,200);
}