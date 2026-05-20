# Environmental Control Unit (ECU) with HMI

## About the Project
This project implements a microcontroller-based Environmental Control Unit (ECU) designed to monitor ambient conditions and manage automated responses. It features a custom Human-Machine Interface (HMI) displayed on an I2C LCD, allowing the user to browse and edit system parameters through a tactile button menu. 

The software architecture was built with automotive and embedded industry standards in mind, focusing on system responsiveness and safety.

## Key Technical Features
* **Finite State Machine (FSM):** The menu navigation and operation modes are strictly controlled via an FSM architecture.
* **Non-blocking Execution:** The main loop runs continuously without `delay()` functions. Timing is handled via software timers (`millis()`).
* **Software Debouncing:** Custom algorithms to filter out mechanical noise from physical button presses.
* **Failsafe Sensor Reading:** Hardware timeouts are implemented for reading sensors (e.g., ultrasonic pulse measurements) to prevent the main execution loop from hanging.
* **Smart Actuation:** Automated control of lighting, irrigation, and security alarms based on real-time ADC (Analog-to-Digital Converter) sensor thresholds.

## Hardware Components
* Microcontroller (Arduino Uno / ATmega328P)
* 16x2 LCD Display (I2C Protocol)
* Sensors: Ultrasonic (Proximity), LDR (Photoresistor), Soil Moisture
* Actuators: LED/Lights, Water Sprinkler/Motor, Buzzer/Alarm
* 3x Tactile Push Buttons (Up, Down, Select)

## Simulation
*You can view and run the live simulation of this circuit on Tinkercad here: https://www.tinkercad.com/things/4iQlftIqMXN-smart-home?sharecode=-QbaPNz9fk0pnkiElzZ_u6EgXod6vt73o_rtn3hcmzY*
