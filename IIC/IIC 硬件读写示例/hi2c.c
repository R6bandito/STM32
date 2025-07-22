#include "hi2c.h" 

void IIC_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef IIC_InitStructure;
	
	// PB10 , PB11 -> I2C2_SCL  I2C2_SDA.
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏输出.
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	IIC_InitStructure.I2C_Mode = I2C_Mode_I2C; // I2C模式.
	IIC_InitStructure.I2C_ClockSpeed = 20000; // 20kHz.
	IIC_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; // 占空比2:1.
	IIC_InitStructure.I2C_Ack = I2C_Ack_Enable; // 允许给从机应答.
	IIC_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 7位地址响应.
	IIC_InitStructure.I2C_OwnAddress1 = 0xF0; // 自身地址.
	I2C_Init(I2C2, &IIC_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);
}

void Mpu6050_Init(void)
{
	IIC_Init();
	Mpu6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	Mpu6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	Mpu6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	Mpu6050_WriteReg(MPU6050_CONFIG, 0x06);
	Mpu6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	Mpu6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

ErrorStatus IIC_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t TimeOut = TIMEOUT_BOUND;
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		TimeOut--;
		if (TimeOut == 0) return ERROR;
	}
	return SUCCESS;
}

void Mpu6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	I2C_GenerateSTART(I2C2, ENABLE); // 生成一个起始位.
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, ADD_MPU6050, I2C_Direction_Transmitter); // 发送设备地址及配置读写模式.
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, RegAddress);
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C2, Data);
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t Mpu6050_ReadReg(uint8_t RegAddress)
{	
	uint8_t RxData;
	
	I2C_GenerateSTART(I2C2, ENABLE);
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, ADD_MPU6050, I2C_Direction_Transmitter);
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, RegAddress);
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C2, ENABLE); // 重新发送起始位.
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, ADD_MPU6050, I2C_Direction_Receiver);
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	I2C_AcknowledgeConfig(I2C2, DISABLE); // 不发送应答位给从机.	
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED); // 出现EV7事件后，说明DR寄存器有数据可以读取了.
	RxData = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	return RxData;
}

void GetSensorData(int16_t *Acc_X, int16_t *Acc_Y, int16_t *Acc_Z, int16_t *Gyro_X, int16_t *Gyro_Y, int16_t *Gyro_Z)
{
	int16_t Data = 0x00;
	Data |= Mpu6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	Data = (Data << 8) | Mpu6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*Acc_X = Data;
	
	Data = 0;
	Data |= Mpu6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	Data = (Data << 8) | Mpu6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*Acc_Y = Data;
	
	Data = 0;
	Data |= Mpu6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	Data = (Data << 8) | Mpu6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*Acc_Z = Data;

	Data = 0;
	Data |= Mpu6050_ReadReg(MPU6050_GYRO_XOUT_H);
	Data = (Data << 8) | Mpu6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*Gyro_X = Data;
	
	Data = 0;
	Data |= Mpu6050_ReadReg(MPU6050_GYRO_YOUT_H);
	Data = (Data << 8) | Mpu6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*Gyro_Y = Data;
	
	Data = 0;
	Data |= Mpu6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	Data = (Data << 8) | Mpu6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*Gyro_Z = Data;
}
