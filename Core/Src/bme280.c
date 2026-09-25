#include "bme280.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

HAL_StatusTypeDef BME280_Init(I2C_HandleTypeDef *hi2c, BME280_Calibration *calib) {
    uint8_t chip_id = 0;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, BME280_ADDR << 1, 0xD0,
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
    calib->dig_T2 = (int16_t)(calib_data[3] << 8 | calib_data[2]);
    calib->dig_T3 = (int16_t)(calib_data[5] << 8 | calib_data[4]);

    // humidity calibrations
    uint8_t dig_H1;

    HAL_I2C_Mem_Read(hi2c,
                     BME280_ADDR << 1,
                     BME280_REG_DIG_H1,
                     I2C_MEMADD_SIZE_8BIT,
                     &dig_H1,
                     1,
                     HAL_MAX_DELAY);

    calib->dig_H1 = dig_H1;

    uint8_t humidity_calib[3];

    HAL_I2C_Mem_Read(hi2c,
                     BME280_ADDR << 1,
                     BME280_REG_DIG_H2,
                     I2C_MEMADD_SIZE_8BIT,
                     humidity_calib,
                     3,
                     HAL_MAX_DELAY);

    calib->dig_H2 = (int16_t)(humidity_calib[1] << 8 | humidity_calib[0]);
    calib->dig_H3 = humidity_calib[2];

    uint8_t humidity_calib_2[4];

    HAL_I2C_Mem_Read(hi2c,
                    BME280_ADDR << 1,
                    BME280_REG_DIG_H4,
                    I2C_MEMADD_SIZE_8BIT,
                    humidity_calib_2,
                    4,
                    HAL_MAX_DELAY);


    calib->dig_H4 = (int16_t)((humidity_calib_2[0] << 4) | (humidity_calib_2[1] & 0x0F));
    calib->dig_H5 = (int16_t)((humidity_calib_2[2] << 4) | (humidity_calib_2[1] >> 4));

    if (calib->dig_H4 & 0x0800) {
        calib->dig_H4 |= 0xF000;
    }

    if (calib->dig_H5 & 0x0800) {
        calib->dig_H5 |= 0xF000;
    }

    calib->dig_H6 = (int8_t)humidity_calib_2[3];

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
    calib->dig_P2 = (int16_t)(press_calib_data[3] << 8 | press_calib_data[2]);
    calib->dig_P3 = (int16_t)(press_calib_data[5] << 8 | press_calib_data[4]);
    calib->dig_P4 = (int16_t)(press_calib_data[7] << 8 | press_calib_data[6]);
    calib->dig_P5 = (int16_t)(press_calib_data[9] << 8 | press_calib_data[8]);
    calib->dig_P6 = (int16_t)(press_calib_data[11] << 8 | press_calib_data[10]);
    calib->dig_P7 = (int16_t)(press_calib_data[13] << 8 | press_calib_data[12]);
    calib->dig_P8 = (int16_t)(press_calib_data[15] << 8 | press_calib_data[14]);
    calib->dig_P9 = (int16_t)(press_calib_data[17] << 8 | press_calib_data[16]);

    // store temperature + humidity configs
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
    calib->t_fine = (int32_t)(var1 + var2);
    float temperature = (var1 + var2) / 5120.0f;

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

float BME280_ReadHumidity(I2C_HandleTypeDef *hi2c, BME280_Calibration *calib) {
    uint8_t data[2];

    HAL_I2C_Mem_Read(hi2c,
                     BME280_ADDR << 1,
                     BME280_REG_HUM_MSB,
                     I2C_MEMADD_SIZE_8BIT,
                     data,
                     2,
                     HAL_MAX_DELAY);

    uint16_t raw_hum = ((uint16_t)data[0] << 8) | data[1];

    float humidity = calib->t_fine - 76800.0f;

    humidity = ((float)raw_hum -
                ((float)calib->dig_H4 * 64.0f +
                 (float)calib->dig_H5 / 16384.0f * humidity))
               *
               ((float)calib->dig_H2 / 65536.0f *
                (1.0f +
                 (float)calib->dig_H6 / 67108864.0f *
                 humidity *
                 (1.0f +
                  (float)calib->dig_H3 / 67108864.0f *
                  humidity)));

    humidity = humidity * (1.0f - (float)calib->dig_H1 * humidity / 524288.0f);

    if (humidity > 100.0f) {
        humidity = 100.0f;
    }

    if (humidity < 0.0f) {
        humidity = 0.0f;
    }

    return humidity; 
}