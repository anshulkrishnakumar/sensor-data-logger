#ifndef BME280_H
#define BME280_H

#include "main.h"

#define BME280_ADDR 0x76

#define BME280_REG_CTRL_HUM     0xF2 // humidity oversampling configurations
#define BME280_REG_CTRL_MEAS    0xF4 // temperature + presssure oversampling configurations
#define BME280_REG_CONFIG       0xF5 // data output rate
#define BME280_REG_DATA         0xF7 // storing data

HAL_StatusTypeDef BME280_Init(I2C_HandleTypeDef *hi2sc);

#endif