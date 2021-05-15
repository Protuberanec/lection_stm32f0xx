/*
 * main.h
 *
 *  Created on: 15 мая 2021 г.
 *      Author: Tr
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f0xx.h"
#include "my_gpio.h"
#include "my_tim.h"
#include "adc.h"
#include "led.h"


void cmd_Init();
void cmd_BlinkLed1(uint8_t* data, uint8_t size);
void cmd_BlinkLed2(uint8_t* data, uint8_t size);
void cmd_Error(uint8_t* data, uint8_t size);


#endif /* MAIN_H_ */
