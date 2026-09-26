#ifndef BME280_H
#define BME280_H

#include "main.h"

#define BME280_ADDR 0x76 // this address was determined by looping through address values to see if a device I2C connection was established

#define BME280_REG_CTRL_HUM     0xF2 // humidity oversampling configurations
#define BME280_REG_CTRL_MEAS    0xF4 // temperature + presssure oversampling configurations
#define BME280_REG_CONFIG       0xF5 // data output rate
#define BME280_REG_DATA         0xF7 // storing data

// temperature value is 20 bits
#define BME280_REG_TEMP_MSB     0xFA // MSB of temperature value, 8 bits
#define BME280_REG_TEMP_LSB     0xFB // middle bits of temperature value, 8 bits
#define BME280_REG_TEMP_XLSB    0xFC // LS bits of temperature value, 4 bits

// humidity value is 16 bits
#define BME280_REG_HUM_MSB  0xFD // 8 bits
#define BME280_REG_HUM_LSB  0xFE // 8 bits

// pressure value is 20 bits
#define BME280_REG_PRESS_MSB  0xF7 // 8 bits
#define BME280_REG_PRESS_LSB  0xF8 // 8 bits
#define BME280_REG_PRESS_XLSB 0xF9 // 4 bits

// registers containing temperature calibration coefficients to convert raw temperature bits to Celsius
#define BME280_REG_DIG_T1       0x88
#define BME280_REG_DIG_T2       0x8A
#define BME280_REG_DIG_T3       0x8C

// registers containing humidity calibration coefficients
#define BME280_REG_DIG_H1  0xA1
#define BME280_REG_DIG_H2  0xE1
#define BME280_REG_DIG_H3  0xE3
#define BME280_REG_DIG_H4  0xE4
#define BME280_REG_DIG_H5  0xE5
#define BME280_REG_DIG_H6  0xE7

// registers containing pressure calibration coefficients
#define BME280_REG_DIG_P1  0x8E
#define BME280_REG_DIG_P2  0x90
#define BME280_REG_DIG_P3  0x92
#define BME280_REG_DIG_P4  0x94
#define BME280_REG_DIG_P5  0x96
#define BME280_REG_DIG_P6  0x98
#define BME280_REG_DIG_P7  0x9A
#define BME280_REG_DIG_P8  0x9C
#define BME280_REG_DIG_P9  0x9E

typedef struct {
    uint16_t dig_T1;
    int16_t dig_T2; 
    int16_t dig_T3;

    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;

    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;

    int32_t t_fine; // temperature correction value that is used for pressure calculation
} BME280_Calibration;


HAL_StatusTypeDef BME280_Init(I2C_HandleTypeDef *hi2sc, BME280_Calibration *calib);
float BME280_ReadTemperature(I2C_HandleTypeDef *hi2c, BME280_Calibration *calib);
float BME280_ReadPressure(I2C_HandleTypeDef *hi2c, BME280_Calibration *calib);
float BME280_ReadHumidity(I2C_HandleTypeDef *hi2c, BME280_Calibration *calib);

#endif