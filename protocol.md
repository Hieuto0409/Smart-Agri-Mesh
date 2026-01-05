# Communication Protocol Design

## 1. Overview
This document defines the data format for communication between **STM32 Sensor Nodes** and the **ESP32 Gateway** via UART.

## 2. Data Format (JSON)
The system uses JSON for its flexibility and readability. Each packet includes:
- `id`: Unique identifier for each node.
- `soil`: Soil moisture percentage (float).
- `temp`: Ambient temperature (float).
- `batt`: Battery level (0-100%).

## 3. Frame Structure
To ensure data integrity, each JSON packet is wrapped with markers:
- **Start Marker:** `!`
- **End Marker:** `#`
