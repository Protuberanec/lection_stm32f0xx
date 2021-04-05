#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

#include <stm32f0xx.h>

#define TEST (*(uint16_t*)0x0800FC00)


void WriteToFlash(uint32_t mem_adr, uint16_t *data, int size_data);
void ReadFromFlash(uint32_t mem, uint16_t* data);

void ErasePage(uint32_t mem_page /*page address*/);


#endif /* MEM_H_INCLUDED */
