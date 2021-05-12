/*
 * adc.h
 *
 *  Created on: Apr 14, 2021
 *      Author: Protuberanec
 */

#ifndef ADC_H_
#define ADC_H_

#include <stm32f0xx.h>

void GPIO_init();
void ADC_init();

void DMA_ADC_init();

#endif /* ADC_H_ */
