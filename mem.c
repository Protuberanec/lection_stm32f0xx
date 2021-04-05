#include "mem.h"

//static FLASH_TypeDef *mFLASH = (FLASH_TypeDef *) FLASH_BASE;    //is not applicable!!!... DON"t USE it...

static void SetProtection() {
    FLASH->CR = FLASH_CR_LOCK;    //
}

static void UnsetProtection() {
    //remove protection for read/write need write a key in a flash_cr
    if ((FLASH->CR & FLASH_CR_LOCK) != 0) {
        FLASH->KEYR = 0x45670123;
        FLASH->KEYR = 0xCDEF89AB;
    }
}

void ErasePage(uint32_t mem_page) {
    UnsetProtection();

    FLASH->CR = FLASH_CR_PER;
    FLASH->AR = mem_page;
    FLASH->CR |= FLASH_CR_STRT;

    while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY);
    if ((FLASH->SR & FLASH_SR_EOP) != 0) {
        FLASH->SR = FLASH_SR_EOP;
    }

    SetProtection();
}

void WriteToFlash(uint32_t mem_adr, uint16_t *data, int size_data){
    ErasePage(mem_adr);
    UnsetProtection();

    FLASH->CR = FLASH_CR_PG;

    for (int i = 0; i < size_data; i++) {
        *(__IO uint16_t*)(mem_adr + i*2) = data[i];
    //    while((mFLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY) {}
        while ((FLASH->SR & FLASH_SR_BSY) != 0);
        if ((FLASH->SR & FLASH_SR_EOP) == 0) {
            FLASH->SR |= FLASH_SR_EOP;
        }
    }
    FLASH->CR &= ~FLASH_CR_PG;

    SetProtection();
}
void ReadFromFlash(uint32_t mem, uint16_t* data){
    *data= *(uint16_t*)(mem);
}




