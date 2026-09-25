#ifndef MPU6050_H
#define MPU6050_H

#include "main.h"

#define MPU6050_ADDR 0x68

#define MPU6050_REG_WHO_AM_I        0x75
#define MPU6050_REG_PWR_MGMT_1      0x6B
#define MPU6050_REG_ACCEL_XOUT_H    0x3B // stores the high byte of accel x data... following bytes are high/low for y and z

typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;

    float gyro_x;
    float gyro_y;
    float gyro_z;

    float temperature;
} MPU6050_Data;

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_Read(I2C_HandleTypeDef *hi2c, MPU6050_Data *data);

#endif