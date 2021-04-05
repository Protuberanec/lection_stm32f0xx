/*
 * my_tim.h
 *
 *  Created on: Mar 29, 2021
 *      Author: Protuberanec
 */

#ifndef MY_TIM_H_
#define MY_TIM_H_

#include <stm32f0xx.h>

void InitTim6();

uint16_t ReturnCNT_TIM6();
uint16_t ReturnMax_TIM6();



#endif /* MY_TIM_H_ */
