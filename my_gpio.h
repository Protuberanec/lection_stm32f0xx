/*
 * my_gpio.h
 *
 *  Created on: Mar 29, 2021
 *      Author: Protuberanec
 */

#ifndef MY_GPIO_H_
#define MY_GPIO_H_

#include <stm32f0xx.h>

#define SET_PC(num_pin, val) (GPIOC->BSRR = (val == 0) ? 1 << (num_pin + 16) : (1 << num_pin))

void InitGPIOC();
void SetResetPinPC(uint8_t num_pin, uint8_t val);

void GenerateTestSigPC7();

void InitGPIOA();


#endif /* MY_GPIO_H_ */
