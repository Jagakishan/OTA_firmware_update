#include <flash_operations.h>
#include <stdint.h>
#include<string.h>

void flashErase(uint32_t address, uint32_t size){
	FLASH_EraseInitTypeDef eraser;

	HAL_FLASH_Unlock();

	eraser.PageAddress=address;
	eraser.TypeErase= FLASH_TYPEERASE_PAGES;
	eraser.NbPages=size/1024+1;

	uint32_t pageError;

	HAL_FLASHEx_Erase(&eraser, &pageError);

	HAL_FLASH_Lock();
}

HAL_StatusTypeDef flashWrite(uint32_t address, uint8_t *data, uint8_t size){
	HAL_StatusTypeDef status;

	for(uint8_t i=0;i<size;i+=4){
		uint32_t tempWord=0xFFFFFFFF;

		memcpy(&tempWord, &data[i], (size-i>=4)?4:size-i);

		status=HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address+i, tempWord);

		if(status!=HAL_OK){
			return status;
		}

	}

	return status;
}
