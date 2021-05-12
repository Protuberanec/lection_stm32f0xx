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

	USART1->CR2 |= USART_CR2_SWAP;

	USART1->BRR = SystemCoreClock / BAUD;	//must be 0x1A1
//	USART1->BRR = (SystemCoreClock / BAUD) >> 1;	in case over8 = 1

	USART1->CR1 |= USART_CR1_UE;

	USART1->CR1 |= USART_CR1_TXEIE | USART_CR1_RXNEIE;
	NVIC_SetPriority(USART1_IRQn, 9);
	NVIC_EnableIRQ(USART1_IRQn);
}

uint8_t USART1_SendData(uint8_t data) {
	if  (USART1->ISR & USART_ISR_TXE)
	{
		USART1->TDR = data;
		return 1;
	}

	return 0;
}





