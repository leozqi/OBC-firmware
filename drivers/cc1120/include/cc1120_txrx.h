#ifndef DRIVERS_CC1120_INCLUDE_CC1120_TXRX_H
#define DRIVERS_CC1120_INCLUDE_CC1120_TXRX_H

#include <stdint.h>
#include "obc_logging.h"

#include <FreeRTOS.h>
#include <os_semphr.h>
#include <sys_common.h>
#include <FreeRTOSConfig.h>

#define AX25_TOTAL_FLAG_BYTES 2
#define AX25_ADDRESS_BYTES 16
#define AX25_CONTROL_BYTES 2
#define AX25_PID_BYTES 1
#define AX25_FCS_BYTES 2
#define AX25_INFO_BYTES 255
#define AX25_PKT_LEN (AX25_TOTAL_FLAG_BYTES + AX25_ADDRESS_BYTES + AX25_CONTROL_BYTES + AX25_PID_BYTES + AX25_FCS_BYTES + AX25_INFO_BYTES)
#define CC1120_TX_RX_PKT_SIZE AX25_PKT_LEN
#define CC1120_TXRX_INTERRUPT_THRESHOLD 100
#define CC1120_TX_SEMAPHORE_TIMEOUT (TickType_t) 5000
#define CC1120_RX_SEMAPHORE_TIMEOUT (TickType_t) 30000


typedef struct
{
    uint8_t addr;
    uint8_t val;
} register_setting_t;


/**
 * @brief Initializes the semaphores that will be used by cc1120_send and cc1120_receive
 * 
*/
void initTxRxSemaphores(void);

/**
 * @brief Gets the number of bytes queued in the TX FIFO
 *
 * @param numBytes - A pointer to an 8-bit integer to store the number of bytes in
 * @return obc_error_code_t - Whether or not the registe read was successful
 */
obc_error_code_t cc1120GetBytesInTxFifo(uint8_t *numBytes);

/**
 * @brief Gets the state of the CC1120 from the MARCSTATE register
 *
 * @param stateNum - A pointer to an 8-bit integer to store the state in
 * @return obc_error_code_t - Whether or not the register read was successful
 */
obc_error_code_t cc1120GetState(uint8_t *stateNum);

/**
 * @brief Resets CC1120 & initializes transmit mode
 *
 * @return obc_error_code_t - Whether or not the setup was a success
 */
obc_error_code_t cc1120Init(void);

/**
 * @brief Adds the given data to the CC1120 FIFO buffer and transmits
 *
 * @param data - An array of 8-bit data to transmit
 * @param len - The size of the provided array
 * @return obc_error_code_t
 */
obc_error_code_t cc1120Send(uint8_t *data, uint32_t len);

/* RX functions */
/**
 * @brief Gets the number of bytes queued in the RX FIFO
 *
 * @param numBytes - A pointer to an 8-bit integer to store the number of bytes in
 * @return obc_error_code_t - Whether or not the register read was successful
 */
obc_error_code_t cc1120GetBytesInRxFifo(uint8_t *numBytes);

/**
 * @brief Switches the cc1120 to RX mode to receive 278 bytes
 *
 * @param data - an array of 8-bit data with size of atleast 278 where received data is stored
 * @return obc_error_code_t
 */
obc_error_code_t cc1120Receive(uint8_t data[]);

/**
 * @brief Gets the handle of the RX semaphore
 *
 * @return SemaphoreHandle_t - The handle of the RX Semaphore
 */
SemaphoreHandle_t getRxSemaphore(void);

/**
 * @brief Gets the handle of the TX semaphore
 *
 * @return SemaphoreHandle_t - The handle of the TX Semaphore
 */
SemaphoreHandle_t getTxSemaphore(void);

#endif /* DRIVERS_CC1120_INCLUDE_CC1120_TXRX_H */