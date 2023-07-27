#pragma once

#include <stdint.h>

#define CRYSTAL_FREQ 16   // MHz
#define SYS_CLK_FREQ 180  // MHz

//*****************************************************************************
// The starting address of the application.  This must be a multiple of 32K(sector size)
// bytes (making it aligned to a page boundary), and can not be 0 (the first sector is
// boot loader).
//
// The flash image of the boot loader must not be larger than this value.
//*****************************************************************************
#define APP_START_ADDRESS (uint32_t)0x00028000

//*****************************************************************************
// The address to store the update status of the application image
// It contains Application Start Address, Application Image Size, etc
//
//*****************************************************************************
#define APP_STATUS_ADDRESS (uint32_t)0x00010000