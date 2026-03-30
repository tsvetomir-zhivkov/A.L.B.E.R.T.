import { Hono } from "@hono/hono";

import * as turbineController from "./controllers/turbineController.js";
import * as sensorController from "./controllers/sensorController.js";
import * as turbineModeController from "./controllers/turbineModeController.js";
import * as connectionController from "./controllers/connectionController.js";

const app = new Hono();

// GET requests endpoints not matching /api/wind_turbines/:wind_turbineID
app.get("/api/wind_turbines/modes", turbineModeController.readAll); // working mode
app.get("/api/wind_turbines/connections", connectionController.readAll); // connection info

// Wind turbines
app.get("/api/wind_turbines", turbineController.readAll);
app.get("/api/wind_turbines/:wind_turbineID", turbineController.readOne);
app.post("/api/wind_turbines", turbineController.createOne);
app.delete("/api/wind_turbines/:wind_turbineID", turbineController.deleteOne);
app.put("/api/wind_turbines/:wind_turbineID", turbineController.updateOne);

// Sensors
app.get("/api/wind_turbines/:wind_turbineID/sensors", sensorController.readAll);
app.get("/api/wind_turbines/:wind_turbineID/sensors/:sensorID", sensorController.readOne);
app.post("/api/wind_turbines/:wind_turbineID/sensors", sensorController.createOne);
app.delete("/api/wind_turbines/:wind_turbineID/sensors/:sensorID", sensorController.deleteOne);
app.put("/api/wind_turbines/:wind_turbineID/sensors/:sensorID", sensorController.updateOne);

// Wind turbines' modes
app.get("/api/wind_turbines/:wind_turbineID/mode", turbineModeController.readOne);
app.post("/api/wind_turbines/:wind_turbineID/mode/:turbine_mode", turbineModeController.createOne);
app.delete("/api/wind_turbines/:wind_turbineID/mode", turbineModeController.deleteOne);
app.put("/api/wind_turbines/:wind_turbineID/mode/:turbine_mode", turbineModeController.updateOne);

// Wind turbines' connection info
app.get("/api/wind_turbines/:wind_turbineID/connection", connectionController.readOne);
app.post("/api/wind_turbines/:wind_turbineID/connection", connectionController.createOne);
app.delete("/api/wind_turbines/:wind_turbineID/connection", connectionController.deleteOne);
app.put("/api/wind_turbines/:wind_turbineID/connection", connectionController.updateOne);


export default app;
