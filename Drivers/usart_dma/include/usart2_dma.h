/*
 * usart2_dma.h
 *
 *  Created on: Jun 23, 2021
 *      Author: DELL
 */

#ifndef USART_DMA_INCLUDE_USART2_DMA_H_
#define USART_DMA_INCLUDE_USART2_DMA_H_



#endif /* USART_DMA_INCLUDE_USART2_DMA_H_ */
#include <stdint.h>
typedef struct {
//	uint32_t DMA_LSIR;
//	uint32_t DMA_HSIR;
//	uint32_t DMA_LIFCR;
//	uint32_t DMA_HIFCR;
	uint32_t DMA_SxCR;
	uint32_t DMA_SxNDTR;
	uint32_t DMA_SxPAR;
	uint32_t DMA_SxM0AR;
	uint32_t DMA_SxM1AR;
	uint32_t DMA_SxFCR;
} DMA_t;
void receive_data_dma(uint32_t address_buff, uint16_t size_buff);
