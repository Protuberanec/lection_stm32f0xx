/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 9.3.0   2021-03-29

The MIT License (MIT)
Copyright (c) 2019 STMicroelectronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************
*/

/* Includes */
#include "stm32f0xx.h"
#include "my_gpio.h"
#include "my_tim.h"
#include "adc.h"
/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

uint8_t status = 0;
void TIM6_DAC_IRQHandler(void) {
	TIM6->SR &= ~TIM_SR_UIF;
	++status;
}

void SpecBlinkLed() {
	switch(status) {
		case 0:
			SetResetPinPC(8, 1);
		break;
		case 1 :
			SetResetPinPC(9, 0);
		break;

		case 2 :
			SetResetPinPC(8, 0);
		break;

		case 3 :
			SetResetPinPC(9, 1);
			status = 0xFF;
		break;
	}
}

void blink_led() {
	BlinkLed(8);
	BlinkLed(9);
}

void GenSig1() {
	blink_led();
	for (int i = 0; i < 100000; i++) {
//		GenerateTestSigPC7();
	}
}

void GenSig2() {
	if (TIM6->SR & TIM_SR_UIF) {
		blink_led();
		TIM6->SR &= ~TIM_SR_UIF;
	}
}

uint16_t data_adc;

void InitSysClock_48Mhz() {
//	1. Disable the PLL by setting PLLON to 0.
	RCC->CR &= ~RCC_CR_PLLON;
//	2. Wait until PLLRDY is cleared. The PLL is now fully stopped.
	while(RCC->CR & RCC_CR_PLLRDY);
//	3. Change the desired parameter.
	RCC->CFGR |= RCC_CFGR_PLLMUL12;
//	4. Enable the PLL again by setting PLLON to 1.
	RCC->CR |= RCC_CR_PLLON;
//	5. Wait until PLLRDY is set.
	while(RCC->CR & RCC_CR_PLLRDY);

	RCC->CFGR |= RCC_CFGR_SW_1;
	for (int i = 0; i < 100; i++);
	SystemCoreClockUpdate();
}

uint8_t data[2] = {0x65, 0x82};
void USART_TX_SendData() {
	static int i = 0;
	if (USART1_SendData(data[i % 2]) == 1) {
		++i;
	}
}
void BlinkLed_via_usart(uint8_t* data, uint8_t size) {
	GPIOC->ODR ^= GPIO_ODR_8;
}

void (*ptrCmd[256])(uint8_t*, uint8_t) = {BlinkLed};

void execute_cmd() {
	uint8_t data;
	if (Buffer_GetFromFront(&data)) {
		return;
	}

	static int status = 0;
	static uint8_t cmd[8];
	static uint8_t ptr = 0;
	switch(status) {
	case 0 :
		if (data == 0x22) {
			++status;
			ptr = 0;
			cmd[ptr++] = data;
		}
		break;
	case 1 :
		cmd[ptr++] = data;
		if (ptr > 7) {
			//check xor
			ptrCmd[cmd[1]](&cmd[0], 8);

		}
		break;
	}
}


uint8_t data_temp_tx[128];
void SendTestDataUSARTDMA() {

	if (DMA1_Channel2->CNDTR == 0) {
		USART1_SendDataViaDma(&data_temp_tx[0], 128);
	}
	else {
		int a = 10;
		a = 100 + 100;
	}
}

int main(void)
{
	InitSysClock_48Mhz();

	USART1_init();
	for (int i = 0; i < 128; i++) {
		data_temp_tx[i] = i+1;
	}


	while (1) {
//		execute_cmd();
		SendTestDataUSARTDMA();
	}
}


