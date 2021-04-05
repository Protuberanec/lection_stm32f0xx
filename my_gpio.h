/*
 * my_gpio.h
 *
 *  Created on: Mar 29, 2021
 *      Author: Protuberanec
 */

#ifndef MY_GPIO_H_
#define MY_GPIO_H_

#include <stm32f0xx.h>

void InitGPIOC();
void SetResetPinPC(uint8_t num_pin, uint8_t val);
void BlinkLed(uint8_t num_pin);
void GenerateTestSigPC7();

void InitGPIOA();


#endif /* MY_GPIO_H_ */
