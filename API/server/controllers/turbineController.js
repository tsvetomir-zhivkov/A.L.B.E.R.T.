// Import the repository to communicate with the database
import * as turbineRepository from "../repositories/turbineRepository.js";

// Using universal function names for ease when creating the handlers.

export const readAll = async (c) => {
    // Fetches all rows from the table
    const res = await turbineRepository.readAllWindTurbines();

    // If there are not rows in the table, return code 204
    if (!res) {
        return c.json({message: "No wind turbines found"}, 204);
    }

    // If at least one wind turbine exists, then return code 200
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
    const res = await turbineRepository.readOneWindTurbine(wtID);

    // If res is undefined, return code 404 - not found
    if (!res) {
        return c.json({message: "Wind turbine not found"}, 404);
    }

    // Returns the required wind turbine if found
    return c.json(res, 200);
}

export const createOne = async (c) => {
    // Uses the given data from the json object/file
    const wt = await c.req.json()

    // Checks if the required data is given
    if (!wt.name || !wt.height || !wt.rotor_diameter || !wt.weight) {
        return c.json({message: "Missing data about the wind turbine"}, 400);
    }

    const res = await turbineRepository.createWindTurbine(wt);

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
    const res = await turbineRepository.deleteWindTurbine(wtID);

    // Checks if the row existed before deletion
    if (!res) {
        return c.json({message: "Wind turbine not found"}, 404);
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

    let wt = {};
    if (header == "application/json") {
        // Uses the given data from the json object/file
        wt = await c.req.json();
    }
    // Updates the specific row and returns it
    const res = await turbineRepository.updateWindTurbine(wtID, wt);

    if (!res) {
        return c.json({message: "Wind turbine not found"}, 404);
    }

    return c.json(res,200);
}