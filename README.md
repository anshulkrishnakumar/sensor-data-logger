# Sensor Data Logger

STM32-based sensor data logger written in C using STM32 HAL.

- Reads temperature, pressure, and humidity from a BME280 over I²C.
- Reads acceleration, gyroscope, and temperature data from an MPU6050-compatible sensor over I²C.
- Uses a timer interrupt to sample sensors at **10 Hz** and buffers **10 samples** in RAM.
- Uses UART for firmware diagnostics and sensor output.
- Configured SPI1/SPI2 and FATFS for planned microSD data logging.
