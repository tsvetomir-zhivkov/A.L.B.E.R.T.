CREATE TABLE sensor_logs(
	sensor_id INTEGER NOT NULL,
	turbine_id INTEGER NOT NULL,
	measurement NUMERIC NOT NULL,
	created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL,
	FOREIGN KEY (sensor_id, turbine_id) REFERENCES sensors(sensor_id, turbine_id)
);

CREATE INDEX idx_lastMeasurementPerSensor ON sensor_logs(sensor_id, turbine_id, created_at);
