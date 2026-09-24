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

    // temperature calibrations
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

    // pressure calibrations
    uint8_t press_calib_data[18];

    HAL_I2C_Mem_Read(hi2c,
                     BME280_ADDR << 1,
                     BME280_REG_DIG_P1,
                     I2C_MEMADD_SIZE_8BIT,
                     press_calib_data,
                     18,
                     HAL_MAX_DELAY);
    
    calib->dig_P1 = (uint16_t)(press_calib_data[1] << 8 | press_calib_data[0]);
    calib->dig_P2 = (uint16_t)(press_calib_data[3] << 8 | press_calib_data[2]);
    calib->dig_P3 = (uint16_t)(press_calib_data[5] << 8 | press_calib_data[4]);
    calib->dig_P4 = (uint16_t)(press_calib_data[7] << 8 | press_calib_data[6]);
    calib->dig_P5 = (uint16_t)(press_calib_data[9] << 8 | press_calib_data[8]);
    calib->dig_P6 = (uint16_t)(press_calib_data[11] << 8 | press_calib_data[10]);
    calib->dig_P7 = (uint16_t)(press_calib_data[13] << 8 | press_calib_data[12]);
    calib->dig_P8 = (uint16_t)(press_calib_data[15] << 8 | press_calib_data[14]);
    calib->dig_P9 = (uint16_t)(press_calib_data[17] << 8 | press_calib_data[16]);

    char press_msg[50];

    snprintf(press_msg, sizeof(press_msg), "dig_P1: %d\r\n", calib->dig_P1);
    HAL_UART_Transmit(&huart2, (uint8_t *)press_msg, strlen(press_msg), HAL_MAX_DELAY);

    snprintf(press_msg, sizeof(press_msg), "dig_P2: %d\r\n", calib->dig_P2);
    HAL_UART_Transmit(&huart2, (uint8_t *)press_msg, strlen(press_msg), HAL_MAX_DELAY);

    snprintf(press_msg, sizeof(press_msg), "dig_P3: %d\r\n", calib->dig_P3);
    HAL_UART_Transmit(&huart2, (uint8_t *)press_msg, strlen(press_msg), HAL_MAX_DELAY);

    snprintf(press_msg, sizeof(press_msg), "dig_P4: %d\r\n", calib->dig_P4);
    HAL_UART_Transmit(&huart2, (uint8_t *)press_msg, strlen(press_msg), HAL_MAX_DELAY);

    snprintf(press_msg, sizeof(press_msg), "dig_P5: %d\r\n", calib->dig_P5);
    HAL_UART_Transmit(&huart2, (uint8_t *)press_msg, strlen(press_msg), HAL_MAX_DELAY);

    snprintf(press_msg, sizeof(press_msg), "dig_P6: %d\r\n", calib->dig_P6);
    HAL_UART_Transmit(&huart2, (uint8_t *)press_msg, strlen(press_msg), HAL_MAX_DELAY);

    snprintf(press_msg, sizeof(press_msg), "dig_P7: %d\r\n", calib->dig_P7);
    HAL_UART_Transmit(&huart2, (uint8_t *)press_msg, strlen(press_msg), HAL_MAX_DELAY);

    snprintf(press_msg, sizeof(press_msg), "dig_P8: %d\r\n", calib->dig_P8);
    HAL_UART_Transmit(&huart2, (uint8_t *)press_msg, strlen(press_msg), HAL_MAX_DELAY);

    snprintf(press_msg, sizeof(press_msg), "dig_P9: %d\r\n", calib->dig_P9);
    HAL_UART_Transmit(&huart2, (uint8_t *)press_msg, strlen(press_msg), HAL_MAX_DELAY);

    // send configs through uart
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

float BME280_ReadTemperature(I2C_HandleTypeDef *hi2c, BME280_Calibration *calib) {
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
    
    // celsius calculation as per BME280 datasheet
    float var1 = (((float)raw_temp / 16384.0f) - ((float)calib->dig_T1 / 1024.0f)) * (float)calib->dig_T2;
    float var2 = (((float)raw_temp / 131072.0f) - ((float)calib->dig_T1 / 8192.0f));
    var2 = var2 * var2 * (float)calib->dig_T3;
    calib->t_fine = var1 + var2;
    float temperature = calib->t_fine / 5120.0f;

    return temperature;
}

// for now, temperature must be read before pressure because t_fine's value depends on initial temperature values 

float BME280_ReadPressure(I2C_HandleTypeDef *hi2c, BME280_Calibration *calib) {
    uint8_t data[3];
    
    HAL_I2C_Mem_Read(hi2c,
                     BME280_ADDR << 1,
                     BME280_REG_PRESS_MSB,
                     I2C_MEMADD_SIZE_8BIT,
                     data,
                     3,
                     HAL_MAX_DELAY);
    
    uint32_t raw_press = ((uint32_t)data[0] << 12) |
                         ((uint32_t)data[1] << 4) |
                         ((uint32_t)data[2] >> 4);


    char msg[50];

    snprintf(msg, sizeof(msg),
            "Raw pressure: %ld\r\n",
            raw_press);

    HAL_UART_Transmit(&huart2,
                    (uint8_t *)msg,
                    strlen(msg),
                    HAL_MAX_DELAY);

    snprintf(msg, sizeof(msg),
            "t_fine: %ld\r\n",
            calib->t_fine);

    HAL_UART_Transmit(&huart2,
                    (uint8_t *)msg,
                    strlen(msg),
                    HAL_MAX_DELAY);

    

    // pascal calculation as per BME280 datasheet
    float var1 = ((float)calib->t_fine / 2.0f) - 64000.0f;
    float var2 = var1 * var1 * ((float)calib->dig_P6) / 32768.0f;
    var2 = var2 + var1 * ((float)calib->dig_P5) * 2.0f;
    var2 = (var2 / 4.0f) + ((float)calib->dig_P4 * 65536.0f);
    var1 = (((float)calib->dig_P3) * var1 * var1 / 524288.0f + ((float)calib->dig_P2) * var1) / 524288.0f;
    var1 = (1.0f + var1 / 32768.0f) * (float)calib->dig_P1;
    if (var1 == 0.0f) {
        return 0.0f;
    }
    float pressure = 1048576.0f - (float)raw_press;
    pressure = (pressure - (var2 / 4096.0f)) * 6250.0f / var1;
    var1 = ((float)calib->dig_P9) * pressure * pressure / 2147483648.0f;
    var2 = pressure * ((float)calib->dig_P8) / 32768.0f;
    pressure = pressure + (var1 + var2 + (float)calib->dig_P7) / 16.0f;

    return pressure;
}