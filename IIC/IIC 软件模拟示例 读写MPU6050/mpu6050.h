#ifndef __MPU_H__
#define __MPU_H__

#include "stm32f10x.h"
#include "i2c.h"
#include "OLED.h"

#define ADD_MPU6050_A  0xD0

// 以下是MPU6050各寄存器地址的映射.
#define MPU6050_PWR_MGMT_1  0x6B
#define MPU6050_PWR_MGMT_2  0x6C
#define MPU6050_WHO_AM_I  0x75

#define MPU6050_SMPLRT_DIV  0x19
#define MPU6050_CONFIG  0x1A
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG 0x1C

#define MPU6050_ACCEL_XOUT_H  0x3B
#define MPU6050_ACCEL_XOUT_L  0x3C
#define MPU6050_ACCEL_YOUT_H  0x3D
#define MPU6050_ACCEL_YOUT_L  0x3E
#define MPU6050_ACCEL_ZOUT_H  0x3F
#define MPU6050_ACCEL_ZOUT_L  0x40
#define MPU6050_TEMP_OUT_H  0x41
#define MPU6050_TEMP_OUT_L  0x42
#define MPU6050_GYRO_XOUT_H  0x43
#define MPU6050_GYRO_XOUT_L  0x44
#define MPU6050_GYRO_YOUT_H  0x45
#define MPU6050_GYRO_YOUT_L  0x46
#define MPU6050_GYRO_ZOUT_H  0x47
#define MPU6050_GYRO_ZOUT_L  0x48

void Mpu_Init(void); // 初始化MPU6050.

void Mpu_WriteReg_P(uint8_t RegAddress, uint8_t Data); // 指定地址写寄存器.

uint8_t Mpu_ReadReg_P(uint8_t ReadAddress); // 指定地址读寄存器.

uint8_t Mpu_ReadReg_N(void); // 当前地址读寄存器.

void Mpu_GetSensorData(int16_t *Acc_X, int16_t *Acc_Y, int16_t *Acc_Z, int16_t *Gyro_X, int16_t *Gyro_Y, int16_t *Gyro_Z); // 获取MPU6050芯片的传感器数据.

#endif // __MPU_H__
