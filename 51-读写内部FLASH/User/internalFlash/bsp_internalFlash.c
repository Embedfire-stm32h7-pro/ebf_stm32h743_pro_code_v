/**
  ******************************************************************************
  * @file    bsp_internalFlash.c
  * @author  fire
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   内部FLASH读写测试范例
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32H743 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./internalFlash/bsp_internalFlash.h"   
#include "./usart/bsp_debug_usart.h"

/*准备写入的测试数据，必须32字节对齐*/
ALIGN_32BYTES(uint32_t DATA_32 = 0x87654321);

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* 要擦除内部FLASH的起始地址 */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_2   
/* 要擦除内部FLASH的结束地址 */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_5  


static uint32_t GetSector(uint32_t Address);

/**
  * @brief  InternalFlash_Test,对内部FLASH进行读写测试
  * @param  None
  * @retval None
  */
int InternalFlash_Test(void)
{
	/*要擦除的起始扇区(包含)及结束扇区(不包含)，如8-12，表示擦除8、9、10、11扇区*/
	uint32_t FirstSector = 0;
	uint32_t NbOfSectors = 0;
	
	uint32_t SECTORError = 0;
	
	uint32_t Address = 0;

	__IO uint32_t Data32 = 0;
	__IO uint32_t MemoryProgramStatus = 0;
	static FLASH_EraseInitTypeDef EraseInitStruct;
	
	/* FLASH 解锁 ********************************/
	/* 使能访问FLASH控制寄存器 */
	HAL_FLASH_Unlock();
	printf("\r\nFLASH已解锁\r\n");

	FirstSector = GetSector(FLASH_USER_START_ADDR);
	NbOfSectors = GetSector(FLASH_USER_END_ADDR)- FirstSector + 1;
	printf("\r\n起始扇区号：%d，扇区数：%d\r\n", FirstSector, NbOfSectors);
	
	/* 擦除用户区域 (用户区域指程序本身没有使用的空间，可以自定义)**/
	/* Fill EraseInit structure*/
	printf("\r\n开始擦除扇区\r\n");
	EraseInitStruct.Banks         = FLASH_BANK_1;
	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;/* 以“字”的大小进行操作 */ 
	EraseInitStruct.Sector        = FirstSector;
	EraseInitStruct.NbSectors     = NbOfSectors;
	/* 开始擦除操作 */
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
		printf("\r\n扇区擦除失败\r\n");
		/*擦除出错，返回，实际应用中可加入处理 */
		return -1;
	}
	else
		printf("\r\n扇区擦除成功\r\n");
	
	/* 以“字”的大小为单位写入数据 ********************************/
	printf("\r\n开始写入数据\r\n");
	Address = FLASH_USER_START_ADDR;
	while (Address < FLASH_USER_END_ADDR)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, Address, (uint64_t)&DATA_32) == HAL_OK)
		{
		  Address = Address + 32;
		}
		else
		{ 
			printf("\r\n扇区写入失败\r\n");
		  /*写入出错，返回，实际应用中可加入处理 */
				return -1;
		}
	}
	printf("\r\n扇区写入成功\r\n");

	/* 给FLASH上锁，防止内容被篡改*/
	HAL_FLASH_Lock(); 
	printf("\r\nFLASH已锁定\r\n");

	/* 从FLASH中读取出数据进行校验***************************************/
	/*  MemoryProgramStatus = 0: 写入的数据正确
	  MemoryProgramStatus != 0: 写入的数据错误，其值为错误的个数 */
	printf("\r\n开始校验数据\r\n");
	Address = FLASH_USER_START_ADDR;
	MemoryProgramStatus = 0;

	while (Address < FLASH_USER_END_ADDR)
	{
		Data32 = *(__IO uint32_t*)Address;
		
		if (Data32 != DATA_32)
		{
			MemoryProgramStatus++;  
		}

		Address = Address + 32;
	}  
	/* 数据校验不正确 */
	if(MemoryProgramStatus)
	{
		return -1;
	}
	else /*数据校验正确*/
	{
		printf("\r\n数据校验成功\r\n");
		return 0;
	}
}

/**
  * @brief  根据输入的地址给出它所在的sector
  *					例如：
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address：地址
  * @retval 地址所在的sector
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
  {
    sector = FLASH_SECTOR_7;  
  }
  return sector;
}


