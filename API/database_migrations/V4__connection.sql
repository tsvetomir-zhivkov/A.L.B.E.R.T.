CREATE TYPE connection_status AS ENUM ('on', 'off');

CREATE TABLE connected(
	turbine_id INTEGER NOT NULL REFERENCES wind_turbines(turbine_id) ON DELETE CASCADE,
	connection connection_status NOT NULL
);
