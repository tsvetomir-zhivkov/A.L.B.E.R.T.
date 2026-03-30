import { Hono } from "@hono/hono";

import * as turbineController from "./controllers/turbineController.js";
import * as sensorController from "./controllers/sensorController.js";

const app = new Hono();

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


export default app;
