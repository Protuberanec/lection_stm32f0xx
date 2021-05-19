/*
 * usart.c
 *
 *  Created on: May 12, 2021
 *      Author: Protuberanec
 */

#include "usart.h"

static uint8_t data = 0;



void USART1_IRQHandler() {
	if (USART1->ISR & USART_ISR_RXNE) {
		data = USART1->RDR;
		Buffer_AddToEnd(data);
	}
	else if (USART1->ISR & USART_ISR_TXE) {
		if (data > 254) {
			USART1->CR1 &= ~USART_CR1_TXEIE;
		}
		USART1->TDR = data++;
	}

	if (USART1->ISR & USART_ISR_RXNE) {
		data = USART1->RDR;
		Buffer_AddToEnd(data);
	}
	else if (USART1->ISR & USART_ISR_TXE) {
		if (data > 254) {
			USART1->CR1 &= ~USART_CR1_TXEIE;
		}
		USART1->TDR = data++;
	}
}

void DMA1_Channel2_3_IRQHandler() {
//start dma for tx....
	if (DMA1->ISR & DMA_ISR_TCIF2) {
		DMA1->IFCR |= DMA_IFCR_CTCIF2;
	}

	if (DMA1->ISR & DMA_ISR_HTIF2) {
		DMA1->IFCR |= DMA_IFCR_CHTIF2;
	}
//end dma for tx
	if (DMA1->ISR & DMA_ISR_TCIF3) {
		DMA1->IFCR |= DMA_IFCR_CTCIF3;
	}

	if (DMA1->ISR & DMA_ISR_HTIF3) {
		DMA1->IFCR |= DMA_IFCR_CHTIF3;
	}
}

uint8_t test_data[SIZE_TX_DATA];

void USART1_DmaTXInit() {
	//tx
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel2->CMAR = &test_data[0];
	DMA1_Channel2->CPAR = (uint32_t*)(&USART1->TDR);
	DMA1_Channel2->CNDTR = SIZE_TX_DATA;
	DMA1_Channel2->CCR |= DMA_CCR_DIR;	//read from memory
	DMA1_Channel2->CCR |= DMA_CCR_MINC;	//increment memory
	DMA1_Channel2->CCR |= DMA_CCR_HTIE | DMA_CCR_TCIE;	//interruption

//	DMA1_Channel2->CCR |= DMA_CCR_CIRC;

	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 9);
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

	DMA1_Channel2->CCR |= DMA_CCR_EN;	//start send data!!!
}

void USART1_DmaRXInit() {
	//rx
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel3->CMAR = &test_data[0];
	DMA1_Channel3->CPAR = (uint32_t*)(&USART1->RDR);
	DMA1_Channel3->CNDTR = SIZE_TX_DATA;
	DMA1_Channel3->CCR &= ~DMA_CCR_DIR;	//read from memory
	DMA1_Channel3->CCR |= DMA_CCR_MINC;	//increment memory
	DMA1_Channel3->CCR |= DMA_CCR_HTIE | DMA_CCR_TCIE;	//interruption

//	DMA1_Channel2->CCR |= DMA_CCR_CIRC;

	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 9);
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

	DMA1_Channel3->CCR |= DMA_CCR_EN;	//start send data!!!
}

static void USART1_gpio_init() {
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;


	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

//	uint32_t temp_moder = GPIOA->MODER;
//	temp_moder |= ....;
//	temp_moder &= ~.....;
//	GPIOA->MODER = temp_moder;

	//pa9 - tx, pa10 - rx
	GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;
	GPIOA->AFR[1]	/*AFRH*/ |= (1 << 4) | (1 << 8);
}

void USART1_init() {
	USART1_gpio_init();
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
//	USART_CR1_OVER8;	//0 - 16, 1 - 8
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;

#ifdef CHECK_PAR
	USART1->CR1 |= USART_CR1_PCE;
	USART1->CR1 |= 1 << 12; 	//set m0 datalength 9 bit
#endif
#ifdef CHECK_BIT_STOP_1_5
	USART1->CR2 |= USART_CR2_STOP_0 | USART_CR2_STOP_1;	//1.5 stop bits
#endif

	USART1->CR3 |= USART_CR3_OVRDIS;
#ifdef DMA_USE
	USART1->CR3 |= USART_CR3_DMAT;
	USART1->CR3 |= USART_CR3_DMAR;
#endif

	USART1->CR2 |= USART_CR2_SWAP;

	USART1->BRR = SystemCoreClock / BAUD;	//must be 0x1A1
//	USART1->BRR = (SystemCoreClock / BAUD) >> 1;	in case over8 = 1

	USART1->CR1 |= USART_CR1_UE;
#ifndef DMA_USE
	USART1->CR1 |= USART_CR1_TXEIE | USART_CR1_RXNEIE;
	NVIC_SetPriority(USART1_IRQn, 9);
	NVIC_EnableIRQ(USART1_IRQn);
#else
	USART1_DmaTXInit();
	USART1_DmaRXInit();
#endif
}

void USART1_SendDataViaDma(uint8_t* data, int size) {
	DMA1_Channel2->CCR &= ~DMA_CCR_EN;
	DMA1_Channel2->CMAR = &data[0];
	DMA1_Channel2->CNDTR = size;
	DMA1_Channel2->CCR |= DMA_CCR_EN;
}

void USART1_GetDataViaDma(uint8_t* data, int size) {
	DMA1_Channel3->CCR &= ~DMA_CCR_EN;
	DMA1_Channel3->CMAR = &data[0];
	DMA1_Channel3->CNDTR = size;
	DMA1_Channel3->CCR |= DMA_CCR_EN;
}


uint8_t USART1_SendData(uint8_t data) {
	if  (USART1->ISR & USART_ISR_TXE)
	{
		USART1->TDR = data;
		return 1;
	}

	return 0;
}





