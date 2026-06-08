# A.L.B.E.R.T.
"Aerodynamical Logical Brutal Electrical Rotating Turbine"

A.L.B.E.R.T. is a small-scale wind turbine control and monitoring system designed for educational and prototyping purposes. The project integrates mechanical design, emebedded systems and cloud-based data handling into a modular architecture.

# Overview
The system consists of two main components

1. Control System (ESP32-based firmware)
2. Dedicated API (data management and communication layer)

Toghether, these components enable wind direction tracking, nacelle positioning, power monitoring, and remote data visualization.

# Control system source code
The control system is implemented on an ESP32 microcontroller and is responsible for real-time operation of the turbine.

# Key features:
- Automatic calibration procedure for nacelle initialization
- Nacelle positioning based on wind direction data from a wind direction sensor
- Voltage measurement of generated power and system supply
- Stepper motor control via TMC2209 driver
- Data transmission to a dedicated API over HTTP
- Task seperation using FreeRTOS to isolate networking from control and UI operationts


# API
The A.L.B.E.R.T. API provides a communication layer between the wind turbine and the monitoring system.

# Responsibilities:
- Receiving and storing wind turbine and sensor data
- Managing generated power measurements
- Monitoring battery status of the system
- Providing structured access to real-time and historical data

# Notes
This is a prototype system intended for educational use and small-scale experimentation. It is not designed for industrial deployment.
