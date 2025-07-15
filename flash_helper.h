/*
	Copyright 2016 - 2021 Benjamin Vedder	benjamin@vedder.se

	This file is part of the VESC firmware.

	The VESC firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The VESC firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#ifndef FLASH_HELPER_H_
#define FLASH_HELPER_H_

#include "conf_general.h"

#define CODE_IND_QML		0
#define CODE_IND_LISP		1
#define CODE_IND_LISP_CONST 2

/*
 * Defines
 */
#define FLASH_SECTORS							16 // Flash has 2 banks of 8x 128k sectors
#define BOOTLOADER_BASE							14
#define APP_BASE								0
#define NEW_APP_BASE							4
#define NEW_APP_SECTORS							4
#define APP_MAX_SIZE							(1024 * 128 * 4 - 8) // Note that the bootloader needs 8 extra bytes

#define QMLUI_BASE								4
#define LISP_BASE								5
#define LISP_CONST_BASE							6


#define PACKAGE_BASE							7

#define QMLUI_MAX_SIZE							(1024 * 128 - 8)
#define LISP_MAX_SIZE							(1024 * 128 - 8)

#define FLASH_BASE_ADDR      (uint32_t)(FLASH_BASE)
#define FLASH_END_ADDR       (uint32_t)(0x081FFFFF)

/* Base address of the Flash sectors Bank 1 */
#define ADDR_FLASH_SECTOR_0_BANK1     ((uint32_t)0x08000000) /* Base @ of Sector 0, 128 Kbytes */
#define ADDR_FLASH_SECTOR_1_BANK1     ((uint32_t)0x08020000) /* Base @ of Sector 1, 128 Kbytes */
#define ADDR_FLASH_SECTOR_2_BANK1     ((uint32_t)0x08040000) /* Base @ of Sector 2, 128 Kbytes */
#define ADDR_FLASH_SECTOR_3_BANK1     ((uint32_t)0x08060000) /* Base @ of Sector 3, 128 Kbytes */
#define ADDR_FLASH_SECTOR_4_BANK1     ((uint32_t)0x08080000) /* Base @ of Sector 4, 128 Kbytes */
#define ADDR_FLASH_SECTOR_5_BANK1     ((uint32_t)0x080A0000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6_BANK1     ((uint32_t)0x080C0000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7_BANK1     ((uint32_t)0x080E0000) /* Base @ of Sector 7, 128 Kbytes */

/* Base address of the Flash sectors Bank 2 */
#define ADDR_FLASH_SECTOR_0_BANK2     ((uint32_t)0x08100000) /* Base @ of Sector 0, 128 Kbytes */
#define ADDR_FLASH_SECTOR_1_BANK2     ((uint32_t)0x08120000) /* Base @ of Sector 1, 128 Kbytes */
#define ADDR_FLASH_SECTOR_2_BANK2     ((uint32_t)0x08140000) /* Base @ of Sector 2, 128 Kbytes */
#define ADDR_FLASH_SECTOR_3_BANK2     ((uint32_t)0x08160000) /* Base @ of Sector 3, 128 Kbytes */
#define ADDR_FLASH_SECTOR_4_BANK2     ((uint32_t)0x08180000) /* Base @ of Sector 4, 128 Kbytes */
#define ADDR_FLASH_SECTOR_5_BANK2     ((uint32_t)0x081A0000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6_BANK2     ((uint32_t)0x081C0000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7_BANK2     ((uint32_t)0x081E0000) /* Base @ of Sector 7, 128 Kbytes */

#define VECTOR_TABLE_ADDRESS					((uint32_t*)FLASH_BASE_ADDR)
#define VECTOR_TABLE_SIZE						((uint32_t)(0x1000))
#define EEPROM_EMULATION_SIZE					((uint32_t)(0x40000))

#define APP_START_ADDRESS						((uint32_t*)(FLASH_BASE_ADDR))
#define APP_SIZE								((uint32_t)(APP_MAX_SIZE)) // eeprom in its own sector - EEPROM_EMULATION_SIZE))

#define	APP_CRC_WAS_CALCULATED_FLAG				((uint32_t)0x00000000)
#define	APP_CRC_WAS_CALCULATED_FLAG_ADDRESS		((uint32_t*)(FLASH_BASE_ADDR + APP_MAX_SIZE - 8))
#define APP_CRC_ADDRESS							((uint32_t*)(FLASH_BASE_ADDR + APP_MAX_SIZE - 4))

// Functions
uint16_t flash_helper_erase_new_app(uint32_t new_app_size);
uint16_t flash_helper_erase_bootloader(void);
uint16_t flash_helper_write_new_app_data(uint32_t offset, uint8_t *data, uint32_t len);

uint16_t flash_helper_erase_code(int ind);
uint16_t flash_helper_write_code(int ind, uint32_t offset, uint8_t *data, uint32_t len);
uint8_t* flash_helper_code_data(int ind);
uint8_t* flash_helper_code_data_raw(int ind);
uint32_t flash_helper_code_size(int ind);
uint16_t flash_helper_code_flags(int ind);

void flash_helper_jump_to_bootloader(void);
uint8_t* flash_helper_get_sector_address(uint32_t fsector);
uint32_t flash_helper_verify_flash_memory(void);
uint32_t flash_helper_verify_flash_memory_chunk(void);

// functions used in vesc_c_if.h and therefore accessible to packages
bool flash_helper_read_nvm(uint8_t *v, unsigned int len, unsigned int address);
bool flash_helper_write_nvm(uint8_t *v, unsigned int len, unsigned int address);
bool flash_helper_wipe_nvm(void);

#endif /* FLASH_HELPER_H_ */
