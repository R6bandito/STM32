#include "stm32f10x.h"
#include "OLED.h"
#include "hi2c.h"

int16_t Acc_X, Acc_Y, Acc_Z, Gyro_X, Gyro_Y, Gyro_Z = 0;

int main(void) 
{
	OLED_Init();
	Mpu6050_Init();
	while(1)
	{
		GetSensorData(&Acc_X, &Acc_Y, &Acc_Z, &Gyro_X, &Gyro_Y, &Gyro_Z);
		OLED_ShowSignedNum(2, 1, Acc_X, 5);
		OLED_ShowSignedNum(3, 1, Acc_Y, 5);
		OLED_ShowSignedNum(4, 1, Acc_Z, 5);
		OLED_ShowSignedNum(2, 8, Gyro_X, 5);
		OLED_ShowSignedNum(3, 8, Gyro_Y, 5);
		OLED_ShowSignedNum(4, 8, Gyro_Z, 5);
	}
}


