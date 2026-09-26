#include "mpu6050.h"
#include <stdint.h>

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t wake = 0x00;

    return HAL_I2C_Mem_Write(hi2c,
                             MPU6050_ADDR << 1,
                             MPU6050_REG_PWR_MGMT_1,
                             I2C_MEMADD_SIZE_8BIT,
                             &wake,
                             1,
                             HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU6050_Read(I2C_HandleTypeDef *hi2c, MPU6050_Data *data)
{
    uint8_t raw[14];

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c,
                                                 MPU6050_ADDR << 1,
                                                 MPU6050_REG_ACCEL_XOUT_H,
                                                 I2C_MEMADD_SIZE_8BIT,
                                                 raw,
                                                 14,
                                                 HAL_MAX_DELAY);

    if (status != HAL_OK) {
        return status;
    }

    int16_t accel_x = (int16_t)((raw[0] << 8) | raw[1]);
    int16_t accel_y = (int16_t)((raw[2] << 8) | raw[3]);
    int16_t accel_z = (int16_t)((raw[4] << 8) | raw[5]);

    int16_t temp_raw = (int16_t)((raw[6] << 8) | raw[7]);

    int16_t gyro_x = (int16_t)((raw[8] << 8) | raw[9]);
    int16_t gyro_y = (int16_t)((raw[10] << 8) | raw[11]);
    int16_t gyro_z = (int16_t)((raw[12] << 8) | raw[13]);

    data->accel_x = accel_x / 16384.0f;
    data->accel_y = accel_y / 16384.0f;
    data->accel_z = accel_z / 16384.0f;

    data->gyro_x = gyro_x / 131.0f;
    data->gyro_y = gyro_y / 131.0f;
    data->gyro_z = gyro_z / 131.0f;

    data->temperature = (temp_raw / 340.0f) + 36.53f;

    return HAL_OK;
}