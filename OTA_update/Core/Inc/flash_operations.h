#ifndef FLASH_DEF
#define FLASH_DEF

#include "main.h"

void flashErase(uint32_t address, uint32_t size);
HAL_StatusTypeDef flashWrite(uint32_t address, uint8_t *data, uint8_t size);

#endif
