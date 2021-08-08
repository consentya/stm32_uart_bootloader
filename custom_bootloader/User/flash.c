/**
 * @file    flash.c
 * @author  Ferenc Nemeth
 * @date    21 Dec 2018
 * @brief   This module handles the memory related functions.
 *
 *          Copyright (c) 2018 Ferenc Nemeth - https://github.com/ferenc-nemeth
 */

#include "flash.h"

/* Function pointer for jumping to user application. */
typedef void (*fnc_ptr)(void);



/* Get sector number by address */
static uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;

	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_SECTOR_0;
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_SECTOR_1;
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_SECTOR_2;
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_SECTOR_3;
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_SECTOR_4;
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_SECTOR_5;
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_SECTOR_6;
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_SECTOR_7;
	}
	else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_SECTOR_8;
	}
	else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
	{
		sector = FLASH_SECTOR_9;
	}
	else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_SECTOR_10;
	}
	else
	{
		sector = FLASH_SECTOR_11;
	}

	return sector;
}

/* Clear flags */
void flash_init(void)
{
	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
			               FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	HAL_FLASH_Lock();
}

/**
 * @brief   This function erases the memory.
 * @param   address: First address to be erased (the last is the end of the flash).
 * @return  status: Report about the success of the erasing.
 */
flash_status flash_erase(uint32_t address)
{
  uint32_t FirstSector, NbOfSectors, SectorError;
  flash_status status = FLASH_ERROR;
  FLASH_EraseInitTypeDef pEraseInit;

  HAL_FLASH_Unlock();
  /* Erase from SECTOR 2~5 */
  FirstSector = GetSector(FLASH_APP_START_ADDRESS);
  NbOfSectors = GetSector(FLASH_APP_END_ADDRESS) - FirstSector;

  pEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
  pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  pEraseInit.Sector = FirstSector;
  pEraseInit.NbSectors = NbOfSectors;

  /* Do the actual erasing. */
  if (HAL_OK == HAL_FLASHEx_Erase(&pEraseInit, &SectorError))
  {
    status = FLASH_OK;
  }

  HAL_FLASH_Lock();

  return status;
}

/**
 * @brief   This function flashes the memory.
 * @param   address: First address to be written to.
 * @param   *data:   Array of the data that we want to write.
 * @param   *length: Size of the array.
 * @return  status: Report about the success of the writing.
 */
flash_status flash_write(uint32_t address, uint32_t *data, uint32_t length)
{
  flash_status status = FLASH_OK;

  HAL_FLASH_Unlock();

  /* Loop through the array. */
  for (uint32_t i = 0u; (i < length) && (FLASH_OK == status); i++)
  {
    /* If we reached the end of the memory, then report an error and don't do anything else.*/
    if (FLASH_APP_END_ADDRESS <= address)
    {
      status |= FLASH_ERROR_SIZE;
    }
    else
    {
      /* The actual flashing. If there is an error, then report it. */
      if (HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data[i]))
      {
        status |= FLASH_ERROR_WRITE;
      }
      /* Read back the content of the memory. If it is wrong, then report an error. */
      if (((data[i])) != (*(volatile uint32_t*)address))
      {
        status |= FLASH_ERROR_READBACK;
      }

      /* Shift the address by a word. */
      address += 4u;
    }
  }

  HAL_FLASH_Lock();

  return status;
}

/**
 * @brief   Actually jumps to the user application.
 * @param   void
 * @return  void
 */
void flash_jump_to_app(void)
{
  /* Function pointer to the address of the user application. */
  fnc_ptr jump_to_app;
  jump_to_app = (fnc_ptr)(*(volatile uint32_t*) (FLASH_APP_START_ADDRESS+4u));

  /* Reset all IPs to default state */
  HAL_DeInit();

  /* Change the main stack pointer. */
  __set_MSP(*(volatile uint32_t*)FLASH_APP_START_ADDRESS);
  jump_to_app();
}

uint32_t  flas_read_autoupdate_flag(void)
{
	return (*(volatile uint32_t*)(FLASH_AUTO_UPDATE_FLAG_ADDRESS)); /* volatile - prevent compiler optimizes code */
}

bool  flash_set_autoupdate_flag(void)
{
	// *(volatile uint32_t*)(FLASH_AUTO_UPDATE_FLAG_ADDRESS) = FLASH_AUTO_UPDATE_MAGIC_NUMBER;
	// if (HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data[i]))

	/* Veriry magic number */
	return ( flas_read_autoupdate_flag() == FLASH_AUTO_UPDATE_MAGIC_NUMBER);
}
