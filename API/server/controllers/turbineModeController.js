// Imports the repository to communicate with the database
import * as turbineModeRepository from "../repositories/turbineModeRepository.js";

// Using universal function names for ease when creating the handlers.

export const readAll = async (c) => {
    // Fetches all rows from the table
    const res = await turbineModeRepository.readAllWindTurbineModes();

    // If there are not rows in the table, return code 200
    if (res.length === 0) {
        return c.json({message: "No working modes found for wind turbines"}, 200);
    }

    // If at least one wind turbine's working mode exists, then return code 200
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
    const res = await turbineModeRepository.readOneWindTurbineMode(wtID);

    // If res is undefined, return code 404 - not found
    if (!res) {
        return c.json({message: "Wind turbine's working mode not found"}, 404);
    }

    // Returns the required wind turbine's working mode if found
    return c.json(res, 200);
}

export const createOne = async (c) => {
    // Uses the parameters provided in the url
    const wtID = Number(await c.req.param("wind_turbineID"));
    const mode = await c.req.param("turbine_mode");

    // Checks whether wtID is a valid number
    if (Number.isNaN(wtID)) {
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    // Checks whether the mode is either auto or manual
    if (mode !== "auto" && mode !== "manual") {
        return c.json({message: "Invalid wind turbine mode"}, 400);
    }
    
    const res = await turbineModeRepository.createWindTurbineMode(wtID, mode);

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
    const res = await turbineModeRepository.deleteWindTurbineMode(wtID);

    // Checks if the row existed before deletion
    if (!res) {
        return c.json({message: "Wind turbine's working mode not found"}, 404);
    }

    return c.json(res, 200);
}

export const updateOne = async (c) => {
    // Uses the parameters provided in the url
    const wtID = Number(await c.req.param("wind_turbineID"));
    const mode = await c.req.param("turbine_mode");

    // Checks whether wtID is a valid number
    if (Number.isNaN(wtID)) {
        // Returns code 400 - bad request   
        return c.json({message: "Invalid wind turbine id"}, 400);
    }

    // Checks whether the mode is either auto or manual
    if (mode !== "auto" && mode !== "manual") {
        return c.json({message: "Invalid wind turbine mode"}, 400);
    }

    // Updates the specific row and returns it
    const res = await turbineModeRepository.updateWindTurbine(wtID, mode);

    if (!res) {
        return c.json({message: "Wind turbine's working mode not found"}, 404);
    }

    return c.json(res,200);
}