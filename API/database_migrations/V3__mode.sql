CREATE TYPE mode_status AS ENUM ('auto', 'manual');

CREATE TABLE turbine_mode (
	turbine_id INTEGER NOT NULL REFERENCES wind_turbines(turbine_id) ON DELETE CASCADE,
	mode mode_status NOT NULL
);
