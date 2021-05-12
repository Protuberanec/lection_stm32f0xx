/*
 * usart.c
 *
 *  Created on: May 12, 2021
 *      Author: Protuberanec
 */

#include "usart.h"

static USART1_gpio_init() {
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
	USART1->CR1 |= USART_CR1_PCE;
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
	USART1->CR1 |= 1 << 12; 	//set m0 datalength 9 bit
	//cr2_msbfirst
	//cr2 stop bits

	USART1->CR3 |= USART_CR3_OVRDIS;

	USART1->BRR = SystemCoreClock / BAUD;	//must be 0x1A1
//	USART1->BRR = (SystemCoreClock / BAUD) >> 1;	in case over8 = 1

	USART1->CR1 |= USART_CR1_UE;
}

uint8_t USART1_SendData(uint8_t data) {
	while (!(USART1->ISR & USART_ISR_TXE));
//	for (int i = 0; i < 1000; i++);

//	if  (USART1->ISR & USART_ISR_TXE)
	{
		USART1->TDR = data;
		return 1;
	}

	return 0;
}





