/*
 * adc.c
 *
 *  Created on: Apr 14, 2021
 *      Author: Protuberanec
 */

#include "adc.h"

#define SIZE_ADC 2048
uint16_t data_adc[SIZE_ADC];

void ADC1_COMP_IRQHandler() {
	ADC1->ISR |= ADC_ISR_EOC;
	GPIOC->ODR ^= GPIO_ODR_8;
}

void DMA1_Channel1_IRQHandler() {
	GPIOC->ODR ^= GPIO_ODR_8;

	if (DMA1->ISR & DMA_ISR_HTIF1) {
		DMA1->IFCR |= DMA_IFCR_CHTIF1;
		float temp = 0;
		GPIOC->ODR |= GPIO_ODR_9;
		for (int i = 0; i < SIZE_ADC/2; i++) {
			temp += (float)data_adc[i]/(float)SIZE_ADC;
		}
//		temp /= (float)SIZE_ADC;
		GPIOC->ODR &= ~GPIO_ODR_9;
	}

	if (DMA1->ISR & DMA_ISR_TCIF1) {
		DMA1->IFCR |= DMA_IFCR_CTCIF1;
		float temp = 0;
		GPIOC->ODR |= GPIO_ODR_9;
		for (int i = SIZE_ADC/2; i < SIZE_ADC; i++) {
			temp += (float)data_adc[i]/(float)SIZE_ADC;
		}
//		temp /= (float)SIZE_ADC;
		GPIOC->ODR &= ~GPIO_ODR_9;
	}
}


void GPIO_init(){
	//pb0 - adc_in 8
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;	//
	GPIOB->MODER |= GPIO_MODER_MODER0;	//analog

}

void TIM15_init_TRG0() {
	RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;

	TIM15->ARR = 20;
	TIM15->PSC = 5;

	TIM15->CR2 |= TIM_CR2_MMS_1;

	TIM15->CR1 |= TIM_CR1_CEN;

}

void ADC_init() {
	int count = 0;
	GPIO_init();
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	RCC->CR2 |= RCC_CR2_HSI14ON;
//		RCC->CIR |= RCC_CIR_HSI14RDYIE;
	while((RCC->CR2 & RCC_CR2_HSI14RDY) != RCC_CR2_HSI14RDY) {
		++count;
	}

	ADC1->CHSELR |= ADC_CHSELR_CHSEL8;
	ADC1->SMPR |= ADC_SMPR_SMP;	//minimal freq conversion
//	ADC1->CFGR1 |= ADC_CFGR1_CONT;	//continuous conversion
	ADC1->CFGR1 |= ADC_CFGR1_DISCEN;
	ADC1->CFGR1 |= ADC_CFGR1_EXTEN_0;	//rising
	ADC1->CFGR1 |= ADC_CFGR1_EXTSEL_2;

	ADC1->IER |= ADC_IER_EOCIE;

	NVIC_SetPriority(ADC1_COMP_IRQn, 10);
	NVIC_EnableIRQ(ADC1_COMP_IRQn);

	ADC1->CR |= ADC_CR_ADEN;	//turn on adc
	ADC1->CR |= ADC_CR_ADSTART;	//turn on adc conversion

	TIM15_init_TRG0();
}

void DMA_ADC_init() {
	int count = 0;
	GPIO_init();
	TIM15_init_TRG0();

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	RCC->CR2 |= RCC_CR2_HSI14ON;
//		RCC->CIR |= RCC_CIR_HSI14RDYIE;
	while((RCC->CR2 & RCC_CR2_HSI14RDY) != RCC_CR2_HSI14RDY) {
		++count;
	}

	ADC1->CHSELR |= ADC_CHSELR_CHSEL8;
	ADC1->SMPR |= ADC_SMPR_SMP;	//minimal freq conversion
//	ADC1->CFGR1 |= ADC_CFGR1_CONT;	//continuous conversion
	ADC1->CFGR1 |= ADC_CFGR1_DISCEN;
	ADC1->CFGR1 |= ADC_CFGR1_EXTEN_0;	//rising
	ADC1->CFGR1 |= ADC_CFGR1_EXTSEL_2;
	ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG;

	//ovrmod - error of ADC, which can stop DMA transfer data
	ADC1->CR |= ADC_CR_ADEN;	//turn on adc
	ADC1->CR |= ADC_CR_ADSTART;	//turn on adc conversion

	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel1->CPAR = 0x00012400 + 0x40000000U + 0x40;	//(uint32_t*)(&ADC1->DR);
	//0x00012400 + 0x40000000U + 0x40
	DMA1_Channel1->CMAR = (uint32_t*)(&data_adc[0]);
	DMA1_Channel1->CNDTR = SIZE_ADC;
	DMA1_Channel1->CCR |= DMA_CCR_PL_1 | DMA_CCR_PL_0;	//DMA_CCR_PL
	DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR_CIRC;
	DMA1_Channel1->CCR &= ~DMA_CCR_DIR;	//from peripheral
	DMA1_Channel1->CCR |= DMA_CCR_TCIE | DMA_CCR_HTIE;	//interruption

	SYSCFG->CFGR1 &= ~SYSCFG_CFGR1_ADC_DMA_RMP;	//for channel 1!!!

	NVIC_SetPriority(DMA1_Ch1_IRQn, 0);
	NVIC_EnableIRQ(DMA1_Ch1_IRQn);

	DMA1_Channel1->CCR |= DMA_CCR_EN;
}
