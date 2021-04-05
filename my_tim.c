/*
 * my_tim.c
 *
 *  Created on: Mar 29, 2021
 *      Author: Protuberanec
 */

#include "my_tim.h"

void InitTim6(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	TIM6->PSC = 1000 - 1;
	TIM6->ARR = 500 - 1;

	TIM6->DIER |= TIM_DIER_UIE;	//turn on interruption
//init NVIC
	NVIC_SetPriority(TIM6_DAC_IRQn /*TIM6_IRQn*/, 8);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
//-----------------------

	TIM6->CR1 |= TIM_CR1_CEN;	//включение таймера
}

uint16_t ReturnCNT_TIM6() {
	return TIM6->CNT;
}

uint16_t ReturnMax_TIM6() {
	return TIM6->ARR;
}
