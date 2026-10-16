# A.L.B.E.R.T. 
## _"Aerodynamical Logical Brutal Electrical Rotating Turbine"_

_A.L.B.E.R.T._ is a small-scale wind turbine with an automatically rotating nacelle and an integrated monitoring system.

>[!NOTE]
> The project integrates mechanical design, emebedded systems and cloud-based data handling into a modular architecture.

## Overview
_A.L.B.E.R.T._ detects the wind direction and automatically rotates its nacelle to face the wind for optimal energy generation. Meanwhile, the ESP32 microcontroller sends both wind and turbine status data to a cloud-based serevr via an API.

The configuration consists of two main components:
* A control System (ESP32-based firmware).
* A dedicated API.

>[!NOTE]
> This is a prototype system intended for educational use and small-scale experimentation. It is not designed for industrial deployment.

## Techniques
* `C/C++`
* `ESP32/Arduino`
* `JavaScript`
* `Svelte`
* `PostgreSQL`
* `Docker`

## Control system
The control system is responsible for real-time operation of the turbine. The core of A.L.B.E.R.T. is an ESP32 microcontroller.

### Key features:
- Automatic calibration procedure for nacelle initialization
- Nacelle positioning based on wind direction data
- Measurement of the generated voltage
- Stepper motor control via TMC2209 driver
- Data transmission to a dedicated server
- Task seperation using FreeRTOS to isolate API communication from control and UI operationts

## API
The cloud-based server API provides a communication layer between the wind turbine and the monitoring system.

### Responsibilities:
- Receives and stores wind turbine data and sensor measurements.
- Monitors the battery status of the system.
- Provides structured access to real-time and historical status data and measurements.

>[!TIP]
> For more technical details, please see the documentation

## Video

[![IMAGE LOADING FAILED :(](https://img.youtube.com/vi/IGIYgQ5NQMg/0.jpg)](https://www.youtube.com/watch?v=IGIYgQ5NQMg)

