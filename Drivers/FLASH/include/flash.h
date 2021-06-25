/*
 * flash_write.h
 *
 *  Created on: Jun 22, 2021
 *      Author: DELL
 */
//void FlashErase(uint8_t starSector, uint8_t numSector)__attribute__((section(".run_in_ram")));
#ifndef FLASH_FLASH_WRITE_H_
#define FLASH_FLASH_WRITE_H_
#endif /* FLASH_FLASH_WRITE_H_ */

#include <stdint.h>
void FlashWrite(uint32_t address, uint32_t data2Write)__attribute__((section(".run_in_ram")));
void FlashErase(uint8_t starSector, uint8_t numSector)__attribute__((section(".run_in_ram")));
