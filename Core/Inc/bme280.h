#ifndef BME280_H
#define BME280_H

#include "main.h"

#define BME280_ADDR 0x76

#define BME280_REG_CTRL_HUM     0xF2 // humidity oversampling configurations
#define BME280_REG_CTRL_MEAS    0xF4 // temperature + presssure oversampling configurations
#define BME280_REG_CONFIG       0xF5 // data output rate
#define BME280_REG_DATA         0xF7 // storing data

// temperature register stores 20 bits
#define BME280_REG_TEMP_MSB     0xFA // MSB of temperature value, 8 bits
#define BME280_REG_TEMP_LSB     0xFB // middle bits of temperature value, 8 bits
#define BME280_REG_TEMP_XLSB    0xFC // LS bits of temperature value, 4 bits

// registers containing temperature calibration coefficients to convert raw temperature bits to Celsius
#define BME280_REG_DIG_T1       0x88
#define BME280_REG_DIG_T2       0x8A
#define BME280_REG_DIG_T3       0x8C

typedef struct {
    uint16_t dig_T1; // baseline reference value - this needs to be positive
    int16_t dig_T2; // T2 and T3 are correction/slope multipliers - they can be negative
    int16_t dig_T3;
} BME280_Calibration;


HAL_StatusTypeDef BME280_Init(I2C_HandleTypeDef *hi2sc, BME280_Calibration *calib);
float BME280_ReadTemperature(I2C_HandleTypeDef *hi2c, BME280_Calibration *calib);

#endif