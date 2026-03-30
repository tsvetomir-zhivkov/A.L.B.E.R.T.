CREATE TYPE mode_status AS ENUM ('auto', 'manual');

CREATE TABLE turbine_mode (
	turbine_id INTEGER PRIMARY KEY NOT NULL REFERENCES wind_turbines(turbine_id) ON DELETE CASCADE,
	mode mode_status DEFAULT 'auto' NOT NULL,
	created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL,
	changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL
);
