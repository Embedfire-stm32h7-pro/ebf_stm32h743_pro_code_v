#ifndef __INTERNAL_FLASH_H
#define	__INTERNAL_FLASH_H

#include "stm32h7xx.h"


/* Base address of the Flash sectors */ 
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 128 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08020000) /* Base address of Sector 1, 128 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08040000) /* Base address of Sector 2, 128 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x08068000) /* Base address of Sector 3, 128 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08080000) /* Base address of Sector 4, 128 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x080A0000) /* Base address of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x080C0000) /* Base address of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x080E0000) /* Base address of Sector 7, 128 Kbytes */



int InternalFlash_Test(void);



#endif /* __INTERNAL_FLASH_H */

