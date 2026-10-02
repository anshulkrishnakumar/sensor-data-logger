STM32 Sensor Data Logger
STM32 firmware project for collecting environmental and motion data from a BME280 and MPU6050-compatible sensor, buffering measurements in RAM, and logging them to a microSD card.
Hardware
- STM32 Nucleo-F446RE
- BME280
- MPU6050-compatible module
- MicroSD card module + microSD card
- Breadboard + jumper wires
Tech Stack
C · STM32 HAL · STM32CubeMX · I²C · SPI · UART · TIM6 · FATFS · CMake · Ninja
Wiring
Wiring diagram coming soon

<!-- Add diagram here -->
<!-- ![Wiring Diagram](docs/wiring-diagram.png) -->

Implemented
- BME280 driver over I²C
  - Temperature
  - Pressure
  - Humidity
- MPU6050-compatible driver over I²C
  - 3-axis acceleration
  - 3-axis gyroscope
  - Sensor temperature
- I²C device detection and sensor initialization
- TIM6 interrupt-based sampling at 10 Hz
- Structured sensor data using SensorSample
- 10-sample RAM buffer
- UART diagnostics at 115200 baud
- SPI and FATFS configuration for microSD storage
- CMake/Ninja build and STM32CubeProgrammer flashing workflow
Data Flow
TIM6 Interrupt
      │
      ▼
Sampling Flag
      │
      ▼
Sensor Acquisition
 ┌────┴────┐
 ▼         ▼
BME280   MPU6050
 └────┬────┘
      ▼
 SensorSample
      │
      ▼
  RAM Buffer
      │
      ▼
 microSD / FATFS
