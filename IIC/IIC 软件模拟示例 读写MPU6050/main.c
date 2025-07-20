#include "stm32f10x.h"
#include "mpu6050.h"
#include "OLED.h"

int16_t AX, AY, AZ, GX, GY, GZ;

int main(void) 
{
	OLED_Init();
	Mpu_Init();
		
	while(1)
	{
		Mpu_GetSensorData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		OLED_ShowSignedNum(1, 1, AX, 5);  OLED_ShowSignedNum(1, 8, GX, 5);
		OLED_ShowSignedNum(2, 1, AY, 5);  OLED_ShowSignedNum(2, 8, GY, 5);
		OLED_ShowSignedNum(3, 1, AZ, 5);  OLED_ShowSignedNum(3, 8, GZ, 5);
		
	}
}


