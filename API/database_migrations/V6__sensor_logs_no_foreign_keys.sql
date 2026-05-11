-- Delete the previous table and index
DROP TABLE IF EXISTS sensor_logs CASCADE;

-- Create the table and index again (the new table does not have foreign keys)
CREATE TABLE sensor_logs(
	sensor_id INTEGER NOT NULL,
	turbine_id INTEGER NOT NULL,
	measurement NUMERIC NOT NULL,
    additional_information TEXT,
	created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL
);

CREATE INDEX idx_lastMeasurementPerSensor ON sensor_logs(sensor_id, turbine_id, created_at);
