#include "bme280.h"

HAL_StatusTypeDef BME280_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t chip_id = 0;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, BME280_ADDR << 1, 0xD0, I2C_MEMADD_SIZE_8BIT, &chip_id, 1, HAL_MAX_DELAY);
    
    if (status != HAL_OK) {
        return status;
    }

    if (chip_id != 0x60) {
        return HAL_ERROR;
    }

    return HAL_OK;

}