#include "stm32f10x.h"                  // Device header

uint32_t MyFLASH_ReadWord(uint32_t Address)//读一个字
{
	return *((__IO uint32_t*)(Address));
}

uint16_t MyFLASH_ReadHalfWord(uint32_t Address)//读半个字
{
	return *((__IO uint16_t*)(Address));
}

uint8_t MyFLASH_ReadByte(uint32_t Address)//读一个字节
{
	return *((__IO uint8_t*)(Address));
}

void MyFLASH_EraseAllPages(void)//全擦除
{
	FLASH_Unlock();//对FLASH进行解锁
	FLASH_EraseAllPages();
	FLASH_Lock();//再锁上
}


void MyFLASH_ErasePages(uint32_t PagesAddress)//页擦除
{
	FLASH_Unlock();//对FLASH进行解锁
	FLASH_ErasePage(PagesAddress);
	FLASH_Lock();//再锁上
}

void MyFLASH_ProgramWord(uint32_t Address,uint32_t Data)//写一个字
{
	FLASH_Unlock();//对FLASH进行解锁
	FLASH_ProgramWord(Address,Data);
	FLASH_Lock();//再锁上
}

void MyFLASH_ProgramHalfWord(uint32_t Address,uint16_t Data)//写半个字
{
	FLASH_Unlock();//对FLASH进行解锁
	FLASH_ProgramHalfWord(Address,Data);
	FLASH_Lock();//再锁上
}
