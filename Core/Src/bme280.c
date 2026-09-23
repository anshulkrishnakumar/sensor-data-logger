#include "bme280.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

HAL_StatusTypeDef BME280_Init(I2C_HandleTypeDef *hi2c, BME280_Calibration *calib) {
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

    // temperature coefficient calibrations
    uint8_t calib_data[6];

    HAL_I2C_Mem_Read(hi2c,
                     BME280_ADDR << 1,
                     BME280_REG_DIG_T1,
                     I2C_MEMADD_SIZE_8BIT,
                     calib_data,
                     6,
                     HAL_MAX_DELAY);

    calib->dig_T1 = (uint16_t)(calib_data[1] << 8 | calib_data[0]); // MSB + LSB
    calib->dig_T2 = (uint16_t)(calib_data[3] << 8 | calib_data[2]);
    calib->dig_T3 = (uint16_t)(calib_data[5] << 9 | calib_data[4]);


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

uint32_t BME280_ReadRawTemperature(I2C_HandleTypeDef *hi2c) {
    uint8_t data[3];

    HAL_I2C_Mem_Read(hi2c,
                     BME280_ADDR << 1,
                     BME280_REG_TEMP_MSB,
                     I2C_MEMADD_SIZE_8BIT,
                     data,
                     3,
                     HAL_MAX_DELAY);
    
    uint32_t raw_temp = ((uint32_t)data[0] << 12) | // shift MSB 12 bits to the left
                        ((uint32_t)data[1] << 4) | // shift LSB 4 bits to the left
                        ((uint32_t)data[2] >> 4); // shift XLSB 4 bits to the right

    return raw_temp;
}