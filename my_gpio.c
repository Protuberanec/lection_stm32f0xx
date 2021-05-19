/*
 * my_gpio.c
 *
 *  Created on: Mar 29, 2021
 *      Author: Protuberanec
 */
#include "my_gpio.h"

void EXTI0_1_IRQHandler(void) {
	EXTI->PR |= EXTI_PR_PIF0;	//reset interruption

	TIM6->PSC += 100;
}

void EXTI2_3_IRQHandler(void) {

}

void EXTI4_15_IRQHandler() {
	if (EXTI->PR & EXTI_PR_PIF12) {

	}

	if (EXTI->PR & EXTI_PR_PIF4) {

	}
}

void InitGPIOA() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER0;	//~(3 << 0) == 0xFC == 1111.1100

	EXTI->IMR |= EXTI_IMR_IM0;	//turn on mask for interrupts
	EXTI->EMR |= EXTI_EMR_EM0;	//for events

	EXTI->RTSR |= EXTI_RTSR_RT0;	//
	EXTI->FTSR |= EXTI_FTSR_FT0;

	SYSCFG->EXTICR[0] |= (3 << 12) | (3 << 0);

	NVIC_SetPriority(EXTI0_1_IRQn, 9);
	NVIC_EnableIRQ(EXTI0_1_IRQn);
}

void SoftwareInterruptionPA0() {
	EXTI->SWIER |= EXTI_SWIER_SWI0;	//software turn on interruption
}

void InitGPIOC() {
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER7_0;
	GPIOC->ODR |= GPIO_ODR_8;
}

/*	input		:	num_pin - номер вывода, а val -- значение
 * 	output		:	устанавливает либо 0,1 на соответсвующий вывыод МК
 * 	description	:	хз
 * 	author		:	Я
 * 	date		:	2021-03-29
 * 	version		:	v1.0
 * 	new features  ---- --
 */
void SetResetPinPC(uint8_t num_pin, uint8_t val) {
	//you can create macros with #define
	GPIOC->BSRR = val == 0 ? 1 << (num_pin + 16) : 1 << num_pin;
}
void BlinkLed(uint8_t num_pin) {
	GPIOC->ODR ^= 1 << num_pin;
}

void GenerateTestSigPC7() {
	static uint8_t val = 0;
	SetResetPinPC(7, val);
	val = val == 0 ? 1 : 0;
}
