#ifndef __MY_FLASH_H
#define __My_FLASH_H



uint32_t MyFLASH_ReadWord(uint32_t Address);//读一个字

uint16_t MyFLASH_ReadHalfWord(uint32_t Address);//读半个字

uint8_t MyFLASH_ReadByte(uint32_t Address);//读一个字节

void MyFLASH_ErasePages(uint32_t PagesAddress);

void MyFLASH_EraseAllPages(void);

void MyFLASH_ProgramWord(uint32_t Address,uint32_t Data);

void MyFLASH_ProgramHalfWord(uint32_t Address,uint16_t Data);


#endif
