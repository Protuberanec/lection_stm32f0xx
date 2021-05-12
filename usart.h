/*
 * usart.h
 *
 *  Created on: May 12, 2021
 *      Author: Protuberanec
 */

#ifndef USART_H_
#define USART_H_

#include <stm32f0xx.h>

#define BAUD	115200

void USART1_init();

uint8_t USART1_SendData(uint8_t data);



#endif /* USART_H_ */
