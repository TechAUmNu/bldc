/*
    ChibiOS - Copyright (C) 2006..2021 Giovanni Di Sirio

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
 * @file    hal_efl_lld.h
 * @brief   STM32G4xx Embedded Flash subsystem low level driver header.
 *
 * @addtogroup HAL_EFL
 * @{
 */

#ifndef HAL_EFL_LLD_H
#define HAL_EFL_LLD_H

#if (HAL_USE_EFL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    STM32H7xx configuration options
 * @{
 */
/**
 * @brief   Suggested wait time during erase operations polling.
 */
#if !defined(STM32_FLASH_WAIT_TIME_MS) || defined(__DOXYGEN__)
#define STM32_FLASH_WAIT_TIME_MS            22
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

//#if !defined(STM32_FLASH_SECTOR_SIZE)
//#error "STM32_FLASH_SECTOR_SIZE not defined in registry"
//#endif
//
//#if !defined(STM32_FLASH_NUMBER_OF_BANKS)
//#error "STM32_FLASH_NUMBER_OF_BANKS not defined in registry"
//#endif
//
//#if !defined(STM32_FLASH_SECTORS_PER_BANK)
//#error "STM32_FLASH_SECTORS_PER_BANK not defined in registry"
//#endif

/* Flash size register. */
#define STM32_FLASH_SECTOR_SIZE             131072U
#define STM32_FLASH_NUMBER_OF_BANKS			2
#define STM32_FLASH_SECTORS_PER_BANK		8
#define STM32_FLASH_SIZE_REGISTER           0x1FF1E880
#define STM32_FLASH_SIZE_SCALE              1024U
#define FLASH_VOLTAGE_RANGE_1        0x00000000U       /*!< Flash program/erase by 8 bits  */
#define FLASH_VOLTAGE_RANGE_2        FLASH_CR_PSIZE_0  /*!< Flash program/erase by 16 bits */
#define FLASH_VOLTAGE_RANGE_3        FLASH_CR_PSIZE_1  /*!< Flash program/erase by 32 bits */
#define FLASH_VOLTAGE_RANGE_4        FLASH_CR_PSIZE    /*!< Flash program/erase by 64 bits */
#define FLASH_BANK_1             0x01U                         /*!< Bank 1   */
#define FLASH_BANK_2             0x02U                         /*!< Bank 2   */


/** @defgroup FLASH_Error_Code FLASH Error Code
  * @brief    FLASH Error Code
  * @{
  */
#define HAL_FLASH_ERROR_NONE         0x00000000U               /*!< No error                              */

#define HAL_FLASH_ERROR_WRP          FLASH_FLAG_WRPERR         /*!< Write Protection Error                */
#define HAL_FLASH_ERROR_PGS          FLASH_FLAG_PGSERR         /*!< Program Sequence Error                */
#define HAL_FLASH_ERROR_STRB         FLASH_FLAG_STRBERR        /*!< Strobe Error                          */
#define HAL_FLASH_ERROR_INC          FLASH_FLAG_INCERR         /*!< Inconsistency Error                   */
#if defined (FLASH_SR_OPERR)
#define HAL_FLASH_ERROR_OPE          FLASH_FLAG_OPERR          /*!< Operation Error                       */
#endif /* FLASH_SR_OPERR */
#define HAL_FLASH_ERROR_RDP          FLASH_FLAG_RDPERR         /*!< Read Protection Error                 */
#define HAL_FLASH_ERROR_RDS          FLASH_FLAG_RDSERR         /*!< Read Secured Error                    */
#define HAL_FLASH_ERROR_SNECC        FLASH_FLAG_SNECCERR       /*!< ECC Single Correction Error           */
#define HAL_FLASH_ERROR_DBECC        FLASH_FLAG_DBECCERR       /*!< ECC Double Detection Error            */
#define HAL_FLASH_ERROR_CRCRD        FLASH_FLAG_CRCRDERR       /*!< CRC Read Error                        */

#define HAL_FLASH_ERROR_WRP_BANK1    FLASH_FLAG_WRPERR_BANK1   /*!< Write Protection Error on Bank 1      */
#define HAL_FLASH_ERROR_PGS_BANK1    FLASH_FLAG_PGSERR_BANK1   /*!< Program Sequence Error on Bank 1      */
#define HAL_FLASH_ERROR_STRB_BANK1   FLASH_FLAG_STRBERR_BANK1  /*!< Strobe Error on Bank 1                */
#define HAL_FLASH_ERROR_INC_BANK1    FLASH_FLAG_INCERR_BANK1   /*!< Inconsistency Error on Bank 1         */
#if defined (FLASH_SR_OPERR)
#define HAL_FLASH_ERROR_OPE_BANK1    FLASH_FLAG_OPERR_BANK1    /*!< Operation Error on Bank 1             */
#endif /* FLASH_SR_OPERR */
#define HAL_FLASH_ERROR_RDP_BANK1    FLASH_FLAG_RDPERR_BANK1   /*!< Read Protection Error on Bank 1       */
#define HAL_FLASH_ERROR_RDS_BANK1    FLASH_FLAG_RDSERR_BANK1   /*!< Read Secured Error on Bank 1          */
#define HAL_FLASH_ERROR_SNECC_BANK1  FLASH_FLAG_SNECCERR_BANK1 /*!< ECC Single Correction Error on Bank 1 */
#define HAL_FLASH_ERROR_DBECC_BANK1  FLASH_FLAG_DBECCERR_BANK1 /*!< ECC Double Detection Error on Bank 1  */
#define HAL_FLASH_ERROR_CRCRD_BANK1  FLASH_FLAG_CRCRDERR_BANK1 /*!< CRC Read Error on Bank1               */

#define HAL_FLASH_ERROR_WRP_BANK2    FLASH_FLAG_WRPERR_BANK2    /*!< Write Protection Error on Bank 2      */
#define HAL_FLASH_ERROR_PGS_BANK2    FLASH_FLAG_PGSERR_BANK2    /*!< Program Sequence Error on Bank 2      */
#define HAL_FLASH_ERROR_STRB_BANK2   FLASH_FLAG_STRBERR_BANK2   /*!< Strobe Error on Bank 2                */
#define HAL_FLASH_ERROR_INC_BANK2    FLASH_FLAG_INCERR_BANK2    /*!< Inconsistency Error on Bank 2         */
#if defined (FLASH_SR_OPERR)
#define HAL_FLASH_ERROR_OPE_BANK2    FLASH_FLAG_OPERR_BANK2     /*!< Operation Error on Bank 2             */
#endif /* FLASH_SR_OPERR */
#define HAL_FLASH_ERROR_RDP_BANK2    FLASH_FLAG_RDPERR_BANK2    /*!< Read Protection Error on Bank 2       */
#define HAL_FLASH_ERROR_RDS_BANK2    FLASH_FLAG_RDSERR_BANK2    /*!< Read Secured Error on Bank 2          */
#define HAL_FLASH_ERROR_SNECC_BANK2  FLASH_FLAG_SNECCERR_BANK2  /*!< ECC Single Correction Error on Bank 2 */
#define HAL_FLASH_ERROR_DBECC_BANK2  FLASH_FLAG_DBECCERR_BANK2  /*!< ECC Double Detection Error on Bank 2  */
#define HAL_FLASH_ERROR_CRCRD_BANK2  FLASH_FLAG_CRCRDERR_BANK2  /*!< CRC Read Error on Bank2               */

#define HAL_FLASH_ERROR_OB_CHANGE    FLASH_OPTSR_OPTCHANGEERR   /*!< Option Byte Change Error              */
/**
  * @}
  */

/** @defgroup FLASH_Type_Program FLASH Type Program
  * @{
  */
#define FLASH_TYPEPROGRAM_FLASHWORD  0x01U        /*!< Program a flash word at a specified address */
#if defined (FLASH_OPTCR_PG_OTP)
#define FLASH_TYPEPROGRAM_OTPWORD    0x02U        /*!< Program an OTP word at a specified address  */
#endif /* FLASH_OPTCR_PG_OTP */
/**
  * @}
  */

/** @defgroup FLASH_Flag_definition FLASH Flag definition
  * @brief Flag definition
  * @{
  */
#define FLASH_FLAG_BSY                     FLASH_SR_BSY             /*!< FLASH Busy flag */
#define FLASH_FLAG_WBNE                    FLASH_SR_WBNE            /*!< Write Buffer Not Empty flag */
#define FLASH_FLAG_QW                      FLASH_SR_QW              /*!< Wait Queue on flag */
#define FLASH_FLAG_CRC_BUSY                FLASH_SR_CRC_BUSY        /*!< CRC Busy flag */
#define FLASH_FLAG_EOP                     FLASH_SR_EOP             /*!< End Of Program on flag */
#define FLASH_FLAG_WRPERR                  FLASH_SR_WRPERR          /*!< Write Protection Error on flag */
#define FLASH_FLAG_PGSERR                  FLASH_SR_PGSERR          /*!< Program Sequence Error on flag */
#define FLASH_FLAG_STRBERR                 FLASH_SR_STRBERR         /*!< Strobe Error flag */
#define FLASH_FLAG_INCERR                  FLASH_SR_INCERR          /*!< Inconsistency Error on flag */
#if defined (FLASH_SR_OPERR)
#define FLASH_FLAG_OPERR                   FLASH_SR_OPERR           /*!< Operation Error on flag */
#endif /* FLASH_SR_OPERR */
#define FLASH_FLAG_RDPERR                  FLASH_SR_RDPERR          /*!< Read Protection Error on flag */
#define FLASH_FLAG_RDSERR                  FLASH_SR_RDSERR          /*!< Read Secured Error on flag */
#define FLASH_FLAG_SNECCERR                FLASH_SR_SNECCERR        /*!< Single ECC Error Correction on flag */
#define FLASH_FLAG_DBECCERR                FLASH_SR_DBECCERR        /*!< Double Detection ECC Error on flag */
#define FLASH_FLAG_CRCEND                  FLASH_SR_CRCEND          /*!< CRC End of Calculation flag */
#define FLASH_FLAG_CRCRDERR                FLASH_SR_CRCRDERR        /*!< CRC Read Error on bank flag */

#define FLASH_FLAG_BSY_BANK1               FLASH_SR_BSY             /*!< FLASH Bank 1 Busy flag */
#define FLASH_FLAG_WBNE_BANK1              FLASH_SR_WBNE            /*!< Write Buffer Not Empty on Bank 1 flag */
#define FLASH_FLAG_QW_BANK1                FLASH_SR_QW              /*!< Wait Queue on Bank 1 flag */
#define FLASH_FLAG_CRC_BUSY_BANK1          FLASH_SR_CRC_BUSY        /*!< CRC Busy on Bank 1 flag */
#define FLASH_FLAG_EOP_BANK1               FLASH_SR_EOP             /*!< End Of Program on Bank 1 flag */
#define FLASH_FLAG_WRPERR_BANK1            FLASH_SR_WRPERR          /*!< Write Protection Error on Bank 1 flag */
#define FLASH_FLAG_PGSERR_BANK1            FLASH_SR_PGSERR          /*!< Program Sequence Error on Bank 1 flag */
#define FLASH_FLAG_STRBERR_BANK1           FLASH_SR_STRBERR         /*!< Strobe Error on Bank 1 flag */
#define FLASH_FLAG_INCERR_BANK1            FLASH_SR_INCERR          /*!< Inconsistency Error on Bank 1 flag */
#if defined (FLASH_SR_OPERR)
#define FLASH_FLAG_OPERR_BANK1             FLASH_SR_OPERR           /*!< Operation Error on Bank 1 flag */
#endif /* FLASH_SR_OPERR */
#define FLASH_FLAG_RDPERR_BANK1            FLASH_SR_RDPERR          /*!< Read Protection Error on Bank 1 flag */
#define FLASH_FLAG_RDSERR_BANK1            FLASH_SR_RDSERR          /*!< Read Secured Error on Bank 1 flag */
#define FLASH_FLAG_SNECCERR_BANK1          FLASH_SR_SNECCERR        /*!< Single ECC Error Correction on Bank 1 flag */
#define FLASH_FLAG_DBECCERR_BANK1          FLASH_SR_DBECCERR        /*!< Double Detection ECC Error on Bank 1 flag */
#define FLASH_FLAG_CRCEND_BANK1            FLASH_SR_CRCEND          /*!< CRC End of Calculation on Bank 1 flag */
#define FLASH_FLAG_CRCRDERR_BANK1          FLASH_SR_CRCRDERR        /*!< CRC Read error on Bank 1 flag */

#if defined (FLASH_SR_OPERR)
#define FLASH_FLAG_ALL_ERRORS_BANK1       (FLASH_FLAG_WRPERR_BANK1   | FLASH_FLAG_PGSERR_BANK1   | \
                                           FLASH_FLAG_STRBERR_BANK1  | FLASH_FLAG_INCERR_BANK1   | \
                                           FLASH_FLAG_OPERR_BANK1    | FLASH_FLAG_RDPERR_BANK1   | \
                                           FLASH_FLAG_RDSERR_BANK1   | FLASH_FLAG_SNECCERR_BANK1 | \
                                           FLASH_FLAG_DBECCERR_BANK1 | FLASH_FLAG_CRCRDERR_BANK1) /*!< All Bank 1 error flags */
#else
#define FLASH_FLAG_ALL_ERRORS_BANK1       (FLASH_FLAG_WRPERR_BANK1   | FLASH_FLAG_PGSERR_BANK1   | \
                                           FLASH_FLAG_STRBERR_BANK1  | FLASH_FLAG_INCERR_BANK1   | \
                                           FLASH_FLAG_RDPERR_BANK1   | FLASH_FLAG_RDSERR_BANK1   | \
                                           FLASH_FLAG_SNECCERR_BANK1 | FLASH_FLAG_DBECCERR_BANK1 | \
                                           FLASH_FLAG_CRCRDERR_BANK1) /*!< All Bank 1 error flags */
#endif /* FLASH_SR_OPERR */

#define FLASH_FLAG_ALL_BANK1              (FLASH_FLAG_BSY_BANK1      | FLASH_FLAG_WBNE_BANK1     | \
                                           FLASH_FLAG_QW_BANK1       | FLASH_FLAG_CRC_BUSY_BANK1 | \
                                           FLASH_FLAG_EOP_BANK1      | FLASH_FLAG_CRCEND_BANK1   | \
                                           FLASH_FLAG_ALL_ERRORS_BANK1) /*!< All Bank 1 flags */

#define FLASH_FLAG_BSY_BANK2               (FLASH_SR_BSY      | 0x80000000U)        /*!< FLASH Bank 2 Busy flag */
#define FLASH_FLAG_WBNE_BANK2              (FLASH_SR_WBNE     | 0x80000000U)        /*!< Write Buffer Not Empty on Bank 2 flag */
#define FLASH_FLAG_QW_BANK2                (FLASH_SR_QW       | 0x80000000U)        /*!< Wait Queue on Bank 2 flag */
#define FLASH_FLAG_CRC_BUSY_BANK2          (FLASH_SR_CRC_BUSY | 0x80000000U)        /*!< CRC Busy on Bank 2 flag */
#define FLASH_FLAG_EOP_BANK2               (FLASH_SR_EOP      | 0x80000000U)        /*!< End Of Program on Bank 2 flag */
#define FLASH_FLAG_WRPERR_BANK2            (FLASH_SR_WRPERR   | 0x80000000U)        /*!< Write Protection Error on Bank 2 flag */
#define FLASH_FLAG_PGSERR_BANK2            (FLASH_SR_PGSERR   | 0x80000000U)        /*!< Program Sequence Error on Bank 2 flag */
#define FLASH_FLAG_STRBERR_BANK2           (FLASH_SR_STRBERR  | 0x80000000U)        /*!< Strobe Error on Bank 2 flag */
#define FLASH_FLAG_INCERR_BANK2            (FLASH_SR_INCERR   | 0x80000000U)        /*!< Inconsistency Error on Bank 2 flag */
#if defined (FLASH_SR_OPERR)
#define FLASH_FLAG_OPERR_BANK2             (FLASH_SR_OPERR    | 0x80000000U)        /*!< Operation Error on Bank 2 flag */
#endif /* FLASH_SR_OPERR */
#define FLASH_FLAG_RDPERR_BANK2            (FLASH_SR_RDPERR   | 0x80000000U)        /*!< Read Protection Error on Bank 2 flag */
#define FLASH_FLAG_RDSERR_BANK2            (FLASH_SR_RDSERR   | 0x80000000U)        /*!< Read Secured Error on Bank 2 flag */
#define FLASH_FLAG_SNECCERR_BANK2          (FLASH_SR_SNECCERR | 0x80000000U)        /*!< Single ECC Error Correction on Bank 2 flag */
#define FLASH_FLAG_DBECCERR_BANK2          (FLASH_SR_DBECCERR | 0x80000000U)        /*!< Double Detection ECC Error on Bank 2 flag */
#define FLASH_FLAG_CRCEND_BANK2            (FLASH_SR_CRCEND   | 0x80000000U)        /*!< CRC End of Calculation on Bank 2 flag */
#define FLASH_FLAG_CRCRDERR_BANK2          (FLASH_SR_CRCRDERR | 0x80000000U)        /*!< CRC Read error on Bank 2 flag */

#if defined (FLASH_SR_OPERR)
#define FLASH_FLAG_ALL_ERRORS_BANK2       (FLASH_FLAG_WRPERR_BANK2   | FLASH_FLAG_PGSERR_BANK2   | \
                                           FLASH_FLAG_STRBERR_BANK2  | FLASH_FLAG_INCERR_BANK2   | \
                                           FLASH_FLAG_OPERR_BANK2    | FLASH_FLAG_RDPERR_BANK2   | \
                                           FLASH_FLAG_RDSERR_BANK2   | FLASH_FLAG_SNECCERR_BANK2 | \
                                           FLASH_FLAG_DBECCERR_BANK2 | FLASH_FLAG_CRCRDERR_BANK2) /*!< All Bank 2 error flags */
#else
#define FLASH_FLAG_ALL_ERRORS_BANK2       (FLASH_FLAG_WRPERR_BANK2   | FLASH_FLAG_PGSERR_BANK2   | \
                                           FLASH_FLAG_STRBERR_BANK2  | FLASH_FLAG_INCERR_BANK2   | \
                                           FLASH_FLAG_RDPERR_BANK2   | FLASH_FLAG_RDSERR_BANK2   | \
                                           FLASH_FLAG_SNECCERR_BANK2 | FLASH_FLAG_DBECCERR_BANK2 | \
                                           FLASH_FLAG_CRCRDERR_BANK2) /*!< All Bank 2 error flags */
#endif /* FLASH_SR_OPERR */

#define FLASH_FLAG_ALL_BANK2              (FLASH_FLAG_BSY_BANK2      | FLASH_FLAG_WBNE_BANK2     | \
                                           FLASH_FLAG_QW_BANK2       | FLASH_FLAG_CRC_BUSY_BANK2 | \
                                           FLASH_FLAG_EOP_BANK2      | FLASH_FLAG_CRCEND_BANK2   | \
                                           FLASH_FLAG_ALL_ERRORS_BANK2) /*!< All Bank 2 flags */

#define IS_FLASH_FLAG_BANK1(FLAG)        (((FLAG) & FLASH_FLAG_ALL_BANK1) == (FLAG))
#define IS_FLASH_FLAG_BANK2(FLAG)        (((FLAG) & FLASH_FLAG_ALL_BANK2) == (FLAG))


#define __HAL_FLASH_CLEAR_FLAG_BANK1(__FLAG__)    WRITE_REG(FLASH->CCR1, (__FLAG__))

#define __HAL_FLASH_CLEAR_FLAG_BANK2(__FLAG__)    WRITE_REG(FLASH->CCR2, ((__FLAG__) & 0x7FFFFFFFU))


#if defined (DUAL_BANK)
#define __HAL_FLASH_CLEAR_FLAG(__FLAG__)         (IS_FLASH_FLAG_BANK1(__FLAG__) ?  __HAL_FLASH_CLEAR_FLAG_BANK1(__FLAG__) : \
                                                  __HAL_FLASH_CLEAR_FLAG_BANK2(__FLAG__))
#else
#define __HAL_FLASH_CLEAR_FLAG(__FLAG__)         __HAL_FLASH_CLEAR_FLAG_BANK1(__FLAG__)
#endif /* DUAL_BANK */

#define __HAL_FLASH_GET_FLAG_BANK1(__FLAG__)     (READ_BIT(FLASH->SR1, (__FLAG__)) == (__FLAG__))
#define __HAL_FLASH_GET_FLAG_BANK2(__FLAG__)     (READ_BIT(FLASH->SR2, ((__FLAG__) & 0x7FFFFFFFU)) == (((__FLAG__) & 0x7FFFFFFFU)))
#define __HAL_FLASH_GET_FLAG(__FLAG__)           (IS_FLASH_FLAG_BANK1(__FLAG__) ?  __HAL_FLASH_GET_FLAG_BANK1(__FLAG__) : __HAL_FLASH_GET_FLAG_BANK2(__FLAG__))

#define IS_FLASH_PROGRAM_ADDRESS_BANK1(ADDRESS) (((ADDRESS) >= FLASH_BANK1_BASE) && ((ADDRESS) < FLASH_BANK2_BASE))
#define IS_FLASH_PROGRAM_ADDRESS_BANK2(ADDRESS) (((ADDRESS) >= FLASH_BANK2_BASE ) && ((ADDRESS) <= FLASH_END))


/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/* A flash size declaration. */
typedef struct {
  const flash_descriptor_t* desc;
} efl_lld_size_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Low level fields of the embedded flash driver structure.
 */
#define efl_lld_driver_fields                                               \
  /* Flash registers.*/                                                     \
  FLASH_TypeDef             *flash;                                         \
  const flash_descriptor_t  *descriptor;

/**
 * @brief   Low level fields of the embedded flash configuration structure.
 */
#define efl_lld_config_fields                                               \
  /* Dummy configuration, it is not needed.*/                               \
  uint32_t                  dummy

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern EFlashDriver EFLD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void efl_lld_init(void);
  void efl_lld_start(EFlashDriver *eflp);
  void efl_lld_stop(EFlashDriver *eflp);
  const flash_descriptor_t *efl_lld_get_descriptor(void *instance);
  flash_error_t efl_lld_read(void *instance, flash_offset_t offset,
                             size_t n, uint8_t *rp);
  flash_error_t efl_lld_program(void *instance, flash_offset_t offset,
                                size_t n, const uint8_t *pp);
  flash_error_t efl_lld_start_erase_all(void *instance);
  flash_error_t efl_lld_start_erase_sector(void *instance,
                                           flash_sector_t sector);
  flash_error_t efl_lld_query_erase(void *instance, uint32_t *msec);
  flash_error_t efl_lld_verify_erase(void *instance, flash_sector_t sector);

// Hacked in stripped down st functions
  flash_error_t FLASH_WaitForLastOperation(uint8_t Bank);
  void FLASH_Erase_Sector(uint32_t Sector, uint32_t Bank);
  flash_error_t HAL_FLASH_Erase(uint8_t bank, uint8_t start_sector,
  		uint8_t num_sectors);
  flash_error_t HAL_FLASH_Program(uint32_t FlashAddress,
			const uint8_t *DataPointer, uint32_t bytes);
  flash_error_t HAL_FLASH_Unlock(void);
  flash_error_t HAL_FLASH_Lock(void);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_EFL == TRUE */

#endif /* HAL_EFL_LLD_H */

/** @} */
