#include "mpu6050.h"

uint8_t TimeOut_Counter = 0;

void Mpu_Init(void)
{
	IIC_Init();
	
	// 配置MPU6050: 解除睡眠, 选择陀螺仪时钟, 轴无待机, 采样分频为10, 滤波参数最大, 量程最大.
	Mpu_WriteReg_P(MPU6050_PWR_MGMT_1, 0x01);  // 配置电源管理寄存器.
	Mpu_WriteReg_P(MPU6050_PWR_MGMT_2, 0x00);
	Mpu_WriteReg_P(MPU6050_SMPLRT_DIV, 0x09);  // 采样率分频(10分频).
	Mpu_WriteReg_P(MPU6050_CONFIG, 0x06); // 配置寄存器.
	Mpu_WriteReg_P(MPU6050_GYRO_CONFIG, 0x18); // 陀螺仪配置寄存器.
	Mpu_WriteReg_P(MPU6050_ACCEL_CONFIG, 0x18); // 加速度计配置寄存器.
}

void Mpu_WriteReg_P(uint8_t RegAddress, uint8_t Data)
{
	IIC_StartChannel();
	IIC_SendByte(ADD_MPU6050_A);
	IIC_RecvAck(); // ...此处可以继续拓展 对应答位进行判断.
	
	IIC_SendByte(RegAddress);
	IIC_RecvAck();
	
	IIC_SendByte(Data);
	IIC_RecvAck();
	IIC_StopChannel();
}

uint8_t Mpu_ReadReg_P(uint8_t ReadAddress)
{	
	uint8_t RecvData;
	IIC_StartChannel();
	IIC_SendByte(ADD_MPU6050_A);
	IIC_RecvAck();
	
	IIC_SendByte(ReadAddress);
	IIC_RecvAck();
	
	IIC_StartChannel();
	IIC_SendByte(ADD_MPU6050_A | 0x01); // 将读写位由写转换为读.
	IIC_RecvAck();
	
	RecvData = IIC_RecvByte();
	IIC_SendACk(1); // 此处只需读取一个字节 不给从机应答.
	IIC_StopChannel();
	
	return RecvData;
}

uint8_t Mpu_ReadReg_N(void)
{
	uint8_t RecvData;
	IIC_StartChannel();
	IIC_SendByte(ADD_MPU6050_A | 0x01);
	IIC_RecvAck();
	
	RecvData = IIC_RecvByte();
	IIC_SendACk(1);
	IIC_StopChannel();
	
	return RecvData;
}

void Mpu_GetSensorData(int16_t *Acc_X, int16_t *Acc_Y, int16_t *Acc_Z, int16_t *Gyro_X, int16_t *Gyro_Y, int16_t *Gyro_Z)
{
	uint8_t Data_H; // 高位数据.
	uint8_t Data_L; // 低位数据.
	
	Data_H = Mpu_ReadReg_P(MPU6050_ACCEL_XOUT_H);
	Data_L = Mpu_ReadReg_P(MPU6050_ACCEL_XOUT_L);
	*Acc_X = (Data_H << 8) | Data_L;
	
	Data_H = Mpu_ReadReg_P(MPU6050_ACCEL_YOUT_H);
	Data_L = Mpu_ReadReg_P(MPU6050_ACCEL_YOUT_L);
	*Acc_Y = (Data_H << 8) | Data_L;
	
	Data_H = Mpu_ReadReg_P(MPU6050_ACCEL_ZOUT_H);
	Data_L = Mpu_ReadReg_P(MPU6050_ACCEL_ZOUT_L);
	*Acc_Z = (Data_H << 8) | Data_L;
	
	
	Data_H = Mpu_ReadReg_P(MPU6050_GYRO_XOUT_H);
	Data_L = Mpu_ReadReg_P(MPU6050_GYRO_XOUT_L);
	*Gyro_X = (Data_H << 8) | Data_L;
	
	Data_H = Mpu_ReadReg_P(MPU6050_GYRO_YOUT_H);
	Data_L = Mpu_ReadReg_P(MPU6050_GYRO_YOUT_L);
	*Gyro_Y = (Data_H << 8) | Data_L;
	
	Data_H = Mpu_ReadReg_P(MPU6050_GYRO_ZOUT_H);
	Data_L = Mpu_ReadReg_P(MPU6050_GYRO_ZOUT_L);
	*Gyro_Z = (Data_H << 8) | Data_L;
}

