/*
 * usart2_dma.c
 *
 *  Created on: Jun 22, 2021
 *      Author: DELL
 */
#include "usart2_dma.h"
DMA_t *dma1_rx = (uint32_t*)(0x40026000 + 0x10 + 0x18*0x7); // USART2_RX at address of stream 7
void receive_data_dma(uint32_t address_buff, uint16_t size_buff)
{
	/* BEGIN CONFIGURE DMA1 */
		while(dma1_rx->DMA_SxCR == 1);
	//	dma1_rx->DMA_SxCR &= ~0xFFFFFFFF;
		dma1_rx->DMA_SxPAR = (uint32_t)0x40004404;
		dma1_rx->DMA_SxM0AR = address_buff;
		dma1_rx->DMA_SxNDTR &= ~(0xFFFF);
		dma1_rx->DMA_SxNDTR |= size_buff; // total number of data
		dma1_rx->DMA_SxCR &= ~(0b111 << 25); // clear the CHSEL filed
		dma1_rx->DMA_SxCR |= (0b110 << 25); // select the channel 6, USART2_RX
	//	dma1_rx->DMA_SxCR |= (1 << 5); // USART2 is a flow controller
	//	dma1_rx->DMA_SxCR |= ~(0b11 << 16);
	//	dma1_rx->DMA_SxCR |= (0b10 << 16); // PL = 0b10, priority is high
	//	dma1_rx->DMA_SxCR |= (0b00<<11)|(0b00<<13); // PSIZE and MSIZE are byte
		dma1_rx->DMA_SxCR |= (1<<8)|(1<<10); // PICN =1 and MINC =1, increment after each data transfer
	//	dma1_rx->DMA_SxCR |= (1<<10);
	//	dma1_rx->DMA_SxCR |= (0<<20)|(0<<19)|(0<<18); //
	//	ma1_rx->DMA_SxCR |= (0<<20)|(0<<19)|(0<<18);
		dma1_rx->DMA_SxCR |= 1; // EN bit = 1, active the Stream
		/* END CONFIGURE DMA1 */
		uint32_t* UART_CR3 = (uint32_t*)(0x40004414);// receive
		*UART_CR3 |= 1<<6;
}

