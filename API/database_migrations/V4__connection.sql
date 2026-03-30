CREATE TYPE connection_status AS ENUM ('success', 'fail', 'pending');

CREATE TABLE turbine_connection(
	turbine_id INTEGER PRIMARY KEY NOT NULL REFERENCES wind_turbines(turbine_id) ON DELETE CASCADE,
	turbine_status connection_status DEFAULT 'pending' NOT NULL,
	remote_status connection_status DEFAULT 'pending' NOT NULL,
	created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL,
	changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL
);
