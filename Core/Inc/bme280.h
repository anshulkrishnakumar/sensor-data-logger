#ifndef BME280_H
#define BME280_H

#include "main.h"

#define BME280_ADDR 0x76

HAL_StatusTypeDef BME280_Init(I2C_HandleTypeDef *hi2sc);

#endif