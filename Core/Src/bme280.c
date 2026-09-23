#include "bme280.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

HAL_StatusTypeDef BME280_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t chip_id = 0;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c,
                                                BME280_ADDR << 1,
                                                0xD0,
                                                I2C_MEMADD_SIZE_8BIT,
                                                &chip_id,
                                                1,
                                                HAL_MAX_DELAY);
    
    if (status != HAL_OK) {
        return status;
    }

    if (chip_id != 0x60) {
        return HAL_ERROR;
    }

    uint8_t ctrl_hum = 0x01; // humidity oversampling x1
    uint8_t ctrl_meas = 0x27; // temperature x1, pressure x1, sensor normal mode

    HAL_I2C_Mem_Write(hi2c,
                      BME280_ADDR << 1,
                      BME280_REG_CTRL_HUM,
                      I2C_MEMADD_SIZE_8BIT,
                      &ctrl_hum,
                      1,
                      HAL_MAX_DELAY);
                      
    HAL_I2C_Mem_Write(hi2c,
                      BME280_ADDR << 1,
                      BME280_REG_CTRL_MEAS,
                      I2C_MEMADD_SIZE_8BIT,
                      &ctrl_meas,
                      1,
                      HAL_MAX_DELAY);    

    uint8_t reg_value;
    char msg[50];

    HAL_I2C_Mem_Read(hi2c,
                    BME280_ADDR << 1,
                    BME280_REG_CTRL_HUM,
                    I2C_MEMADD_SIZE_8BIT,
                    &reg_value,
                    1,
                    HAL_MAX_DELAY);

    snprintf(msg, sizeof(msg), "CTRL_HUM: 0x%02X\r\n", reg_value);
    HAL_UART_Transmit(&huart2,
                    (uint8_t *)msg,
                    strlen(msg),
                    HAL_MAX_DELAY);

    HAL_I2C_Mem_Read(hi2c,
                    BME280_ADDR << 1,
                    BME280_REG_CTRL_MEAS,
                    I2C_MEMADD_SIZE_8BIT,
                    &reg_value,
                    1,
                    HAL_MAX_DELAY);

    snprintf(msg, sizeof(msg), "CTRL_MEAS: 0x%02X\r\n", reg_value);
    HAL_UART_Transmit(&huart2,
                    (uint8_t *)msg,
                    strlen(msg),
                    HAL_MAX_DELAY);
    
    
    return HAL_OK;
}