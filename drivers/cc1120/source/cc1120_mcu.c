#include "cc1120_mcu.h"
#include "obc_spi_io.h"
#include <stdio.h>

/**
 * @brief Simultaneously sends and receives a byte over CC1120 SPI interface
 * 
 * @param outb - Data to transfer 
 * @param inb - Buffer to store the received byte.
 * @return error code - An error code from obc_errors.h
 */
obc_error_code_t mcuCC1120SpiTransfer(uint8_t outb, uint8_t *inb) {
    spiDAT1_t spiConfig;
    spiConfig.CS_HOLD = false;
    spiConfig.WDEL = false;
    spiConfig.DFSEL = 0;
    spiConfig.CSNR = SPI_CS_NONE;
    return spiTransmitAndReceiveByte(spiREG4, &spiConfig, outb, inb);
}

/**
 * @brief Calls the correct CS assert function based on the MCU selected.
 * 
 * @return error code - An error code from obc_errors.h
 */
obc_error_code_t mcuCC1120CSAssert(void) {
    return assertChipSelect(spiPORT4, 0);
}

/**
 * @brief Calls the correct CS deassert function based on the MCU selected.
 * 
 * @return error code - An error code from obc_errors.h
 */
obc_error_code_t mcuCC1120CSDeassert(void) {
    return deassertChipSelect(spiPORT4, 0);
}
