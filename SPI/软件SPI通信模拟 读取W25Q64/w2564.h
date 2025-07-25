#ifndef __W25Q64_H__
#define __W25Q64_H__

#include "stm32f10x.h"
#include "myspi.h"

// 以下为W25Q64芯片的部分通信指令.
#define W25__CMD_JEDEC_ID  0x9f
#define W25__CMD_WRITE_ENABLE  0x06
#define W25__CMD_WRITE_DISABLE  0x04
#define W25__CMD_READ_STATUS_REG_1  0x05
#define W25__CMD_PAGE_PROGRAM  0x02
#define W25__CMD_SECTOR_ERASE  0x20
#define W25__CMD_READ_DATA  0x03


#define USELESS_DATA  0xff 

void W25Q64_Init(void); // w25q64芯片初始化.

void W25_GetDeviceId(uint8_t *Manuf, uint16_t *DID); // 获取设备ID.

void W25_EnableWri(void); // 写使能.

void W25_Read_Status_Reg(uint8_t *Status); // 读状态寄存器.

void W25__WaitBusy(void);  // 等待芯片准备就绪.

void W25__PageProgram(uint32_t Address, uint8_t* TxDataArray, uint16_t Count); // 页编程. 

void W25__EraseSector(uint32_t Address); // 扇区擦除.

void W25__ReadData(uint32_t Address, uint8_t* RxDataArray, uint32_t Count); // 读取数据.

#endif // __W25Q64_H__
