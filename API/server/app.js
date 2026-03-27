import { Hono } from "@hono/hono";

import * as turbineController from "./controllers/turbineController.js";

const app = new Hono();

app.get("/api/wind_turbines", turbineController.readAll);
app.get("/api/wind_turbines/:wind_turbineID", turbineController.readOne);
app.post("/api/wind_turbines", turbineController.createOne);
app.delete("/api/wind_turbines/:wind_turbineID", turbineController.deleteOne);
app.put("/api/wind_turbines/:wind_turbineID", turbineController.updateOne);


export default app;
