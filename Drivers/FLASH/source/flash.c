/*
 * flash_write.c
 *
 *  Created on: Jun 22, 2021
 *      Author: DELL
 */
#include "flash.h"
void FlashWrite(uint32_t address, uint32_t data2Write)
{
	//HAL_FLASH_Unlock();
	uint32_t *FLASH_MM;
//	uint32_t *ACR = (uint32_t*)0x40023C00;
	uint32_t *SR = (uint32_t*)0x40023C0C;
	uint32_t *CR = (uint32_t*)0x40023C10;
//	*ACR &= ~((1<<9)|(1<<10));
//	*ACR |= (1<<11)|(1<<12);
//	*SR |= 1<<7;
	while(((*SR >> 16) & 1) == 1) {}; // wait for BSY bit = 0
	*CR |= 1; // set the PG bit
	*CR &= ~(1<<1); // SER bit = 0, inactive sector erase
	*CR &= ~(0b11 << 8); // clear the PSIZE field
	*CR |= 0b10 << 8; // set the PSIZE is x64 parallelism
	while(((*SR >> 16) & 1) == 1) {}; // wait for BSY bit = 0
	FLASH_MM = address;
	*FLASH_MM = data2Write;
	while(((*SR >> 0) & 1) == 1) {}; // wait for EOP bit set
	while(((*SR >> 16) & 1) == 1) {}; // wait for BSY bit = 0
}

void FlashErase(uint8_t starSector, uint8_t numSector)
{
uint8_t sector = starSector;
//uint32_t *KEYR = (uint32_t*)0x40023404;
uint32_t *SR = (uint32_t*)0x40023C0C;
uint32_t *CR = (uint32_t*)0x40023C10;
//*KEYR = (uint32_t)0x45670123; // to unlock FLASH_CR register
//while(((*CR >> 16) & 1) == 1) {}; // wait for BSY bit = 0
for(int i = 0; i < numSector; i++)
{
	if(sector < 7)
	{
		while(((*SR >> 16) & 1) == 1) {}; // wait for BSY bit = 0
		*CR &= ~(0b1111 << 3); // clear SNB bit field
		sector = starSector + i; // select the sector need to erase
		*CR |= (sector<<3); // erase the selected sector
		*CR &= ~(0b11 << 8); // claer the PSIZE is x64 parallelism
		*CR |= 0b10 << 8; // set the PSIZE is x64 parallelism
		*CR |= 1<<1; // SER bit = 1, sector erase activated
		*CR |= 1<<16; // STRT bit =1, trigger an erase operation
		while(((*SR >> 16) & 1) == 1) {}; // wait for BSY bit = 0
	}
}
}

