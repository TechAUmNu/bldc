/*
    ChibiOS - Copyright (C) 2006..2019 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    hal_efl_lld.c
 * @brief   STM32G4xx Embedded Flash subsystem low level driver source.
 *
 * @addtogroup HAL_EFL
 * @{
 */

#include <string.h>

#include "hal.h"

#if (HAL_USE_EFL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define STM32_FLASH_LINE_SIZE               (32) // bytes
#define STM32_FLASH_LINE_MASK               (STM32_FLASH_LINE_SIZE - 1U)

#define FLASH_PDKEY1                        0x04152637U
#define FLASH_PDKEY2                        0xFAFBFCFDU

#define FLASH_KEY1                          0x45670123U
#define FLASH_KEY2                          0xCDEF89ABU

#define FLASH_OPTKEY1                       0x08192A3BU
#define FLASH_OPTKEY2                       0x4C5D6E7FU

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   EFL1 driver identifier.
 */
EFlashDriver EFLD1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const flash_descriptor_t efl_lld_desc = {
       /* Bank 1 & 2 (DBM) organisation. */
      .attributes        = FLASH_ATTR_ERASED_IS_ONE |
                           FLASH_ATTR_MEMORY_MAPPED |
                           FLASH_ATTR_ECC_CAPABLE   |
                           FLASH_ATTR_ECC_ZERO_LINE_CAPABLE,
      .page_size         = STM32_FLASH_LINE_SIZE,
      .sectors_count     = STM32_FLASH_SECTORS_PER_BANK * STM32_FLASH_NUMBER_OF_BANKS,
      .sectors           = NULL,
      .sectors_size      = STM32_FLASH_SECTOR_SIZE,
      .address           = (uint8_t *)FLASH_BASE,
      .size              = STM32_FLASH_NUMBER_OF_BANKS * STM32_FLASH_SECTORS_PER_BANK * STM32_FLASH_SECTOR_SIZE
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void stm32_flash_lock_bank1(EFlashDriver *eflp) {
  eflp->flash->CR1 |= FLASH_CR_LOCK;
}

static inline void stm32_flash_lock_bank2(EFlashDriver *eflp) {
  eflp->flash->CR2 |= FLASH_CR_LOCK;
}

static inline void stm32_flash_unlock_bank1(EFlashDriver *eflp) {
  eflp->flash->KEYR1 |= FLASH_KEY1;
  eflp->flash->KEYR1 |= FLASH_KEY2;
}

static inline void stm32_flash_unlock_bank2(EFlashDriver *eflp) {
  eflp->flash->KEYR2 |= FLASH_KEY1;
  eflp->flash->KEYR2 |= FLASH_KEY2;
}

static inline void stm32_flash_enable_pgm_bank1(EFlashDriver *eflp) {
  eflp->flash->CR1 |= FLASH_CR_PG;
}

static inline void stm32_flash_enable_pgm_bank2(EFlashDriver *eflp) {
  eflp->flash->CR2 |= FLASH_CR_PG;
}

static inline void stm32_flash_disable_pgm_bank1(EFlashDriver *eflp) {
  eflp->flash->CR1 &= ~FLASH_CR_PG;
}

static inline void stm32_flash_disable_pgm_bank2(EFlashDriver *eflp) {
  eflp->flash->CR2 &= ~FLASH_CR_PG;
}

static inline void stm32_flash_clear_status(EFlashDriver *eflp) {

  eflp->flash->SR1 = 0x0000FFFFU;
}

static inline void stm32_flash_wait_busy(EFlashDriver *eflp) {

  /* Wait for busy bit clear.*/
  while ((eflp->flash->SR1 & FLASH_SR_BSY) != 0U) {
  }
}

static inline size_t stm32_flash_get_size(void) {
  return *(uint16_t*)((uint32_t) STM32_FLASH_SIZE_REGISTER) * STM32_FLASH_SIZE_SCALE;
}

static inline flash_error_t stm32_flash_check_errors(EFlashDriver *eflp) {
  uint32_t sr = eflp->flash->SR1;

  /* Clearing error conditions.*/
  eflp->flash->SR1 = sr & 0x0000FFFFU;

  //TODO EM: Rework this to have all the errors
  /* Some errors are only caught by assertion.*/
//  osalDbgAssert((sr & (FLASH_SR_FASTERR |
//                       FLASH_SR_MISERR |
//                       FLASH_SR_SIZERR)) == 0U, "unexpected flash error");

  /* Decoding relevant errors.*/
  if ((sr & FLASH_SR_WRPERR) != 0U) {
    return FLASH_ERROR_HW_FAILURE;
  }

//  if ((sr & (FLASH_SR_PGAERR | FLASH_SR_PROGERR | FLASH_SR_OPERR)) != 0U) {
//    return eflp->state == FLASH_PGM ? FLASH_ERROR_PROGRAM : FLASH_ERROR_ERASE;
//  }

  return FLASH_NO_ERROR;
}


/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

// Converted ST Code
flash_error_t HAL_FLASH_Unlock(void)
{
  if(READ_BIT(FLASH->CR1, FLASH_CR_LOCK) != 0U)
  {
    /* Authorize the FLASH Bank1 Registers access */
    WRITE_REG(FLASH->KEYR1, FLASH_KEY1);
    WRITE_REG(FLASH->KEYR1, FLASH_KEY2);

    /* Verify Flash Bank1 is unlocked */
    if (READ_BIT(FLASH->CR1, FLASH_CR_LOCK) != 0U)
    {
      return FLASH_ERROR_HW_FAILURE;
    }
  }
  if(READ_BIT(FLASH->CR2, FLASH_CR_LOCK) != 0U)
  {
    /* Authorize the FLASH Bank2 Registers access */
    WRITE_REG(FLASH->KEYR2, FLASH_KEY1);
    WRITE_REG(FLASH->KEYR2, FLASH_KEY2);

    /* Verify Flash Bank2 is unlocked */
    if (READ_BIT(FLASH->CR2, FLASH_CR_LOCK) != 0U)
    {
      return FLASH_ERROR_HW_FAILURE;
    }
  }
  return FLASH_NO_ERROR;
}

/**
  * @brief  Locks the FLASH control registers access
  * @retval HAL Status
  */
flash_error_t HAL_FLASH_Lock(void)
{
  /* Set the LOCK Bit to lock the FLASH Bank1 Control Register access */
  SET_BIT(FLASH->CR1, FLASH_CR_LOCK);
  /* Verify Flash Bank1 is locked */
  if (READ_BIT(FLASH->CR1, FLASH_CR_LOCK) == 0U)
  {
    return FLASH_ERROR_HW_FAILURE;
  }
  /* Set the LOCK Bit to lock the FLASH Bank2 Control Register access */
  SET_BIT(FLASH->CR2, FLASH_CR_LOCK);

  /* Verify Flash Bank2 is locked */
  if (READ_BIT(FLASH->CR2, FLASH_CR_LOCK) == 0U)
  {
    return FLASH_ERROR_HW_FAILURE;
  }
  return FLASH_NO_ERROR;
}

__attribute__((section(".itcm_text")))
flash_error_t FLASH_WaitForLastOperation(uint8_t Bank) {
	/* Wait for the FLASH operation to complete by polling on QW flag to be reset.
	 Even if the FLASH operation fails, the QW flag will be reset and an error
	 flag will be set */

	uint32_t bsyflag = FLASH_FLAG_QW_BANK1;
	uint32_t errorflag = 0;
	systime_t tickstart = chVTGetSystemTimeX();

	if (Bank == FLASH_BANK_2) {
		/* Select bsyflag depending on Bank */
		bsyflag = FLASH_FLAG_QW_BANK2;
	}
	while (__HAL_FLASH_GET_FLAG(bsyflag)) {
		if (chVTTimeElapsedSinceX(tickstart) > STM32_FLASH_WAIT_TIME_MS * 10) {
			return FLASH_ERROR_TIMEOUT;
		}
	}

	/* Get Error Flags */
	if (Bank == FLASH_BANK_1) {
		errorflag = FLASH->SR1 & FLASH_FLAG_ALL_ERRORS_BANK1;
	} else {
		errorflag = (FLASH->SR2 & FLASH_FLAG_ALL_ERRORS_BANK2) | 0x80000000U;
	}

	/* In case of error reported in Flash SR1 or SR2 register */
	if ((errorflag & 0x7FFFFFFFU) != 0U) {
		//pFlash.ErrorCode |= errorflag;  /*Save the error code*/
		__HAL_FLASH_CLEAR_FLAG(errorflag); /* Clear error programming flags */
		return FLASH_ERROR_HW_FAILURE;
	}

	/* Check FLASH End of Operation flag  */
	if (Bank == FLASH_BANK_1) {
		if (__HAL_FLASH_GET_FLAG_BANK1(FLASH_FLAG_EOP_BANK1)) {
			__HAL_FLASH_CLEAR_FLAG_BANK1(FLASH_FLAG_EOP_BANK1); /* Clear FLASH End of Operation pending bit */
		}
	} else {
		if (__HAL_FLASH_GET_FLAG_BANK2(FLASH_FLAG_EOP_BANK2)) {
			__HAL_FLASH_CLEAR_FLAG_BANK2(FLASH_FLAG_EOP_BANK2); /* Clear FLASH End of Operation pending bit */
		}
	}

	return FLASH_NO_ERROR;
}


__attribute__((section(".itcm_text")))
void FLASH_Erase_Sector(uint32_t Sector, uint32_t Bank) {
	if (Bank == FLASH_BANK_1) {
		/* Reset Program/erase VoltageRange and Sector Number for Bank1 */
		FLASH->CR1 &= ~(FLASH_CR_PSIZE | FLASH_CR_SNB);

		FLASH->CR1 |= (FLASH_CR_SER | FLASH_VOLTAGE_RANGE_3
				| (Sector << FLASH_CR_SNB_Pos) | FLASH_CR_START);
	} else {
		/* Reset Program/erase VoltageRange and Sector Number for Bank2 */
		FLASH->CR2 &= ~(FLASH_CR_PSIZE | FLASH_CR_SNB);

		FLASH->CR2 |= (FLASH_CR_SER | FLASH_VOLTAGE_RANGE_3
				| (Sector << FLASH_CR_SNB_Pos) | FLASH_CR_START);
	}
}


__attribute__((section(".itcm_text")))
flash_error_t HAL_FLASH_Erase(uint8_t bank, uint8_t start_sector,
		uint8_t num_sectors) {

	SCB_DisableICache();
	SCB_DisableDCache();

	flash_error_t status = FLASH_NO_ERROR;

	if (bank == FLASH_BANK_1) {
		if (FLASH_WaitForLastOperation(FLASH_BANK_1) != FLASH_NO_ERROR) {
			return FLASH_ERROR_ERASE;
		}
	} else {
		if (FLASH_WaitForLastOperation(FLASH_BANK_2) != FLASH_NO_ERROR) {
			return FLASH_ERROR_ERASE;
		}
	}
	for (uint8_t i = start_sector; i < (num_sectors + start_sector); i++) {
		FLASH_Erase_Sector(i, bank);

		if (bank == FLASH_BANK_1) {
			/* Wait for last operation to be completed */
			status = FLASH_WaitForLastOperation(FLASH_BANK_1);

			/* If the erase operation is completed, disable the SER Bit */
			FLASH->CR1 &= (~(FLASH_CR_SER | FLASH_CR_SNB));
		} else {
			/* Wait for last operation to be completed */
			status = FLASH_WaitForLastOperation(FLASH_BANK_2);

			/* If the erase operation is completed, disable the SER Bit */
			FLASH->CR2 &= (~(FLASH_CR_SER | FLASH_CR_SNB));
		}
		if (status != FLASH_NO_ERROR) {
			break;
		}
	}

	SCB_EnableICache();
	SCB_EnableDCache();

	return status;
}

// Programs 8 bit
__attribute__((section(".itcm_text")))
flash_error_t HAL_FLASH_Program(uint32_t FlashAddress,
		const uint8_t *DataPointer, uint32_t bytes) {
	flash_error_t status;
	uint32_t bank;

	SCB_DisableICache();
	SCB_DisableDCache();

	if (IS_FLASH_PROGRAM_ADDRESS_BANK1(FlashAddress)) {
		bank = FLASH_BANK_1;
	} else if (IS_FLASH_PROGRAM_ADDRESS_BANK2(FlashAddress)) {
		bank = FLASH_BANK_2;
	}

	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(bank);

	if (status == FLASH_NO_ERROR) {
		if (bank == FLASH_BANK_1) {
			SET_BIT(FLASH->CR1, FLASH_CR_PG);
		} else {
			SET_BIT(FLASH->CR2, FLASH_CR_PG);
		}
		__ISB();
		__DSB();

		/* Actual program implementation.*/
		volatile uint32_t offset = FlashAddress-FLASH_BASE;
		while (bytes > 0U) {
			volatile uint32_t *address;

			union {
				uint32_t w[STM32_FLASH_LINE_SIZE / sizeof(uint32_t)];
				uint8_t b[STM32_FLASH_LINE_SIZE / sizeof(uint8_t)];
			} line;

			/* Unwritten bytes are initialized to all ones.*/
			line.w[0] = 0xFFFFFFFFU;
			line.w[1] = 0xFFFFFFFFU;
			line.w[2] = 0xFFFFFFFFU;
			line.w[3] = 0xFFFFFFFFU;
			line.w[4] = 0xFFFFFFFFU;
			line.w[5] = 0xFFFFFFFFU;
			line.w[6] = 0xFFFFFFFFU;
			line.w[7] = 0xFFFFFFFFU;

			/* Programming address aligned to flash lines.*/
			address = (volatile uint32_t*) (FLASH_BASE
					+ (offset & ~STM32_FLASH_LINE_MASK));

			/* Copying data inside the prepared line.*/
			do {
				line.b[offset & STM32_FLASH_LINE_MASK] = *DataPointer;
				offset++;
				bytes--;
				DataPointer++;
			} while ((bytes > 0U) & ((offset & STM32_FLASH_LINE_MASK) != 0U));

			/* Programming line.*/
			address[0] = line.w[0];
			address[1] = line.w[1];
			address[2] = line.w[2];
			address[3] = line.w[3];
			address[4] = line.w[4];
			address[5] = line.w[5];
			address[6] = line.w[6];
			address[7] = line.w[7];
			status = FLASH_WaitForLastOperation(bank);
			if (status != FLASH_NO_ERROR) {
				break;
			}
		}

		__ISB();
		__DSB();

		/* Wait for last operation to be completed */
		status = FLASH_WaitForLastOperation(bank);
		{
			if (bank == FLASH_BANK_1) {
				/* If the program operation is completed, disable the PG */
				CLEAR_BIT(FLASH->CR1, FLASH_CR_PG);
			} else {
				/* If the program operation is completed, disable the PG */
				CLEAR_BIT(FLASH->CR2, FLASH_CR_PG);
			}
		}
	}

	SCB_EnableICache();
	SCB_EnableDCache();

	return status;
}































/**
 * @brief   Low level Embedded Flash driver initialization.
 *
 * @notapi
 */
void efl_lld_init(void) {

  /* Driver initialization.*/
  eflObjectInit(&EFLD1);
  EFLD1.flash = FLASH;
  EFLD1.descriptor = &efl_lld_desc;
}

/**
 * @brief   Configures and activates the Embedded Flash peripheral.
 *
 * @param[in] eflp      pointer to a @p EFlashDriver structure
 *
 * @notapi
 */
void efl_lld_start(EFlashDriver *eflp) {
  //stm32_flash_unlock(eflp);
  FLASH->CR1 = 0x00000000U;
}

/**
 * @brief   Deactivates the Embedded Flash peripheral.
 *
 * @param[in] eflp      pointer to a @p EFlashDriver structure
 *
 * @notapi
 */
void efl_lld_stop(EFlashDriver *eflp) {

  //stm32_flash_lock(eflp);
}

/**
 * @brief   Gets the flash descriptor structure.
 *
 * @param[in] ip                    pointer to a @p EFlashDriver instance
 * @return                          A flash device descriptor.
 * @retval                          Pointer to single bank if DBM not enabled.
 * @retval                          Pointer to bank1 if DBM enabled.
 *
 * @notapi
 */
const flash_descriptor_t *efl_lld_get_descriptor(void *instance) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  return devp->descriptor;
}

/**
 * @brief   Read operation.
 *
 * @param[in] ip                    pointer to a @p EFlashDriver instance
 * @param[in] offset                offset within full flash address space
 * @param[in] n                     number of bytes to be read
 * @param[out] rp                   pointer to the data buffer
 * @return                          An error code.
 * @retval FLASH_NO_ERROR           if there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_READ         if the read operation failed.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_read(void *instance, flash_offset_t offset,
                           size_t n, uint8_t *rp) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  flash_error_t err = FLASH_NO_ERROR;

  osalDbgCheck((instance != NULL) && (rp != NULL) && (n > 0U));

  const flash_descriptor_t *bank = efl_lld_get_descriptor(instance);
  osalDbgCheck((size_t)offset + n <= (size_t)bank->size);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No reading while erasing.*/
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* FLASH_READ state while the operation is performed.*/
  devp->state = FLASH_READ;

  /* Clearing error status bits.*/
  stm32_flash_clear_status(devp);

  /* Actual read implementation.*/
  memcpy((void *)rp, (const void *)efl_lld_get_descriptor(instance)->address
                                   + offset, n);

  /* Checking for errors after reading.*/
  // TODO EM: not required because of ECC memory?
//  if ((devp->flash->SR1 & FLASH_SR_RDERR) != 0U) {
//    err = FLASH_ERROR_READ;
//  }

  /* Ready state again.*/
  devp->state = FLASH_READY;

  return err;

}

/**
 * @brief   Program operation.
 * @note    The device supports ECC. It is only possible to write erased
 *          pages once except when writing all zeroes to a location.
 *
 * @param[in] ip                    pointer to a @p EFlashDriver instance
 * @param[in] offset                offset within full flash address space
 * @param[in] n                     number of bytes to be programmed
 * @param[in] pp                    pointer to the data buffer
 * @return                          An error code.
 * @retval FLASH_NO_ERROR           if there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_PROGRAM      if the program operation failed.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_program(void *instance, flash_offset_t offset,
                              size_t n, const uint8_t *pp) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  const flash_descriptor_t *bank = efl_lld_get_descriptor(instance);
  flash_error_t err = FLASH_NO_ERROR;

  osalDbgCheck((instance != NULL) && (pp != NULL) && (n > 0U));
  osalDbgCheck((size_t)offset + n <= (size_t)bank->size);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No programming while erasing.*/
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* FLASH_PGM state while the operation is performed.*/
  devp->state = FLASH_PGM;

  /* Clearing error status bits.*/
  stm32_flash_clear_status(devp);

  /* Enabling PGM mode in the controller.*/
  //stm32_flash_enable_pgm(devp);

  /* Actual program implementation.*/
  while (n > 0U) {
    volatile uint32_t *address;

    union {
      uint32_t  w[STM32_FLASH_LINE_SIZE / sizeof (uint32_t)];
      uint8_t   b[STM32_FLASH_LINE_SIZE / sizeof (uint8_t)];
    } line;

    /* Unwritten bytes are initialized to all ones.*/
    line.w[0] = 0xFFFFFFFFU;
    line.w[1] = 0xFFFFFFFFU;
    line.w[2] = 0xFFFFFFFFU;
	line.w[3] = 0xFFFFFFFFU;
	line.w[4] = 0xFFFFFFFFU;
	line.w[5] = 0xFFFFFFFFU;
	line.w[6] = 0xFFFFFFFFU;
	line.w[7] = 0xFFFFFFFFU;

    /* Programming address aligned to flash lines.*/
    address = (volatile uint32_t *)(bank->address +
                                    (offset & ~STM32_FLASH_LINE_MASK));

    /* Copying data inside the prepared line.*/
    do {
      line.b[offset & STM32_FLASH_LINE_MASK] = *pp;
      offset++;
      n--;
      pp++;
    }
    while ((n > 0U) & ((offset & STM32_FLASH_LINE_MASK) != 0U));

    /* Programming line.*/
    address[0] = line.w[0];
    address[1] = line.w[1];
    address[2] = line.w[2];
	address[3] = line.w[3];
	address[4] = line.w[4];
	address[5] = line.w[5];
	address[6] = line.w[6];
	address[7] = line.w[7];

    stm32_flash_wait_busy(devp);
    err = stm32_flash_check_errors(devp);
    if (err != FLASH_NO_ERROR) {
      break;
    }
  }

  /* Disabling PGM mode in the controller.*/
  //stm32_flash_disable_pgm(devp);

  /* Ready state again.*/
  devp->state = FLASH_READY;

  return err;
}

/**
 * @brief   Starts a whole-device erase operation.
 * @note    This function only erases bank 2 if it is present. Bank 1 is not
 *          allowed since it is normally where the primary program is located.
 *          Pages on bank 1 can be individually erased.
 *
 * @param[in] ip                    pointer to a @p EFlashDriver instance
 * @return                          An error code.
 * @retval FLASH_NO_ERROR           if there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_start_erase_all(void *instance) {
  /* Mass erase not allowed. */
  return FLASH_ERROR_UNIMPLEMENTED;
}

/**
 * @brief   Starts an sector erase operation.
 *
 * @param[in] ip                    pointer to a @p EFlashDriver instance
 * @param[in] sector                sector to be erased
 *                                  this is an index within the total sectors
 *                                  in a flash bank
 * @return                          An error code.
 * @retval FLASH_NO_ERROR           if there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_start_erase_sector(void *instance,
                                         flash_sector_t sector) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  const flash_descriptor_t *bank = efl_lld_get_descriptor(instance);
  osalDbgCheck(instance != NULL);
  osalDbgCheck(sector < bank->sectors_count);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No erasing while erasing.*/
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* FLASH_PGM state while the operation is performed.*/
  devp->state = FLASH_ERASE;

  /* Clearing error status bits.*/
  stm32_flash_clear_status(devp);

if (sector < (bank->sectors_count / 2)) {
	/* Enable sector erase.*/
	devp->flash->CR1 |= FLASH_CR_SER;
	/* Mask off the sector selection bits.*/
	devp->flash->CR1 &= ~FLASH_CR_SNB;

	/* Set the sector selection bits.*/
	devp->flash->CR1 |= sector << FLASH_CR_SNB_Pos;

	/* Start the erase.*/
	devp->flash->CR1 |= FLASH_CR_START;
}
else {
	/* Second bank. Adjust sector index. */
	sector -= (bank->sectors_count / 2);
	/* Enable sector erase.*/
	devp->flash->CR2 |= FLASH_CR_SER;

	/* Mask off the sector selection bits.*/
	devp->flash->CR2 &= ~FLASH_CR_SNB;

	/* Set the sector selection bits.*/
	devp->flash->CR2 |= sector << FLASH_CR_SNB_Pos;

	/* Start the erase.*/
	devp->flash->CR2 |= FLASH_CR_START;
}


  return FLASH_NO_ERROR;
}

/**
 * @brief   Queries the driver for erase operation progress.
 *
 * @param[in] ip                    pointer to a @p EFlashDriver instance
 * @param[out] msec                 recommended time, in milliseconds, that
 *                                  should be spent before calling this
 *                                  function again, can be @p NULL
 * @return                          An error code.
 * @retval FLASH_NO_ERROR           if there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_ERASE        if the erase operation failed.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @api
 */
flash_error_t efl_lld_query_erase(void *instance, uint32_t *msec) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  flash_error_t err;

  /* If there is an erase in progress then the device must be checked.*/
  if (devp->state == FLASH_ERASE) {

    /* Checking for operation in progress.*/
    if ((devp->flash->SR1 & FLASH_SR_BSY) == 0U) {

    	// TODO EM: Fix
//      /* Disabling the various erase control bits.*/
//      devp->flash->CR1 &= ~(FLASH_CR_MER1 |
//#if defined(FLASH_CR_MER2)
//                           FLASH_CR_MER2 |
//#endif
//                           FLASH_CR_PER);

      /* No operation in progress, checking for errors.*/
      err = stm32_flash_check_errors(devp);

      /* Back to ready state.*/
      devp->state = FLASH_READY;
    }
    else {
      /* Recommended time before polling again. This is a simplified
         implementation.*/
      if (msec != NULL) {
        *msec = (uint32_t)STM32_FLASH_WAIT_TIME_MS;
      }

      err = FLASH_BUSY_ERASING;
    }
  }
  else {
    err = FLASH_NO_ERROR;
  }

  return err;
}

/**
 * @brief   Returns the erase state of a sector.
 *
 * @param[in] ip                    pointer to a @p EFlashDriver instance
 * @param[in] sector                sector to be verified
 * @return                          An error code.
 * @retval FLASH_NO_ERROR           if the sector is erased.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_VERIFY       if the verify operation failed.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_verify_erase(void *instance, flash_sector_t sector) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  uint32_t *address;
  const flash_descriptor_t *bank = efl_lld_get_descriptor(instance);
  flash_error_t err = FLASH_NO_ERROR;
  unsigned i;

  osalDbgCheck(instance != NULL);
  osalDbgCheck(sector < bank->sectors_count);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No verifying while erasing.*/
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* Address of the sector in the bank.*/
  address = (uint32_t *)(bank->address +
                        flashGetSectorOffset(getBaseFlash(devp), sector));

  /* FLASH_READ state while the operation is performed.*/
  devp->state = FLASH_READ;

  /* Scanning the sector space.*/
  uint32_t sector_size = flashGetSectorSize(getBaseFlash(devp), sector);
  for (i = 0U; i < sector_size / sizeof(uint32_t); i++) {
    if (*address != 0xFFFFFFFFU) {
      err = FLASH_ERROR_VERIFY;
      break;
    }
    address++;
  }

  /* Ready state again.*/
  devp->state = FLASH_READY;

  return err;
}

#endif /* HAL_USE_EFL == TRUE */

/** @} */
