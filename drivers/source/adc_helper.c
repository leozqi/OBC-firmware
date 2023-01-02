#include "adc_helper.h"

#include <FreeRTOS.h>
#include <os_semphr.h>

/**
 * @brief Function to start conversion of a single adc channel
 * 
 * @param adc Pointer to ADC Module
 *        - adcREG1: ADC1 module pointer
 *        - adcREG2: ADC2 module pointer
 * @param channel ADC channel to be converted
 * @param fifo_size Size of FIFO queue to store converted values
 * @param group ADC group to be converted
 * 
 * @return 1 if successful, 0 if not 
 */ 

static uint8_t adcStartConversion_selChn(adcBASE_t *adc, uint8_t channel, uint8_t fifoSize, uint8_t group) {
    if(adc == NULL) {
        return 0;
    }

    adc->GxINTCR[group] = fifoSize;

    adc->GxSEL[group] = 1 << channel;

    return 1;
}

/**
 * @brief Function to get ADC converted data
 * 
 * @param adc Pointer to ADC Module
 *        - adcREG1: ADC1 module pointer
 *        - adcREG2: ADC2 module pointer
 * @param group ADC group to get converted data from
 * @param data Pointer to store ADC converted data
 * 
 * @return 1 if successful, 0 if not 
 */ 

static uint8_t adcGetSingleData(adcBASE_t *adc, uint8_t group, adcData_t *data) {
    if((adc == NULL) || (data == NULL)) {
        return 0;
    }

    unsigned buf;
    adcData_t *ptr = data;

    buf = adc->GxBUF[group].BUF0;
    ptr->value = (unsigned short)(buf & 0xFFFU);
    ptr->id = (unsigned short)((buf >> 16U) & 0x1FU);

    adc->GxINTFLG[group] = 9U;

    return 1;
}

/**
 * @brief Function to get ADC digital voltage
 * 
 * @param adc Pointer to ADC Module
 *        - adcREG1: ADC1 module pointer
 *        - adcREG2: ADC2 module pointer
 * @param group ADC group to be converted
 * @param channel ADC channel to be converted
 * @param adcData Pointer to converted data
 * 
 * @return 1 if successful, 0 if not 
 */ 

static uint8_t adcDigitalVoltage(adcBASE_t *adc, uint8_t group, uint8_t channel, adcData_t *adcData) {
    if((adc == NULL) || (adcData == NULL)) {
        return 0;
    }
    
    adcStartConversion_selChn(adc, channel, FIFOSIZE, group);

    while(!adcIsConversionComplete(adc, group));

    adcGetSingleData(adc, group, adcData);

    return 1;
}

uint8_t adcAnalogVoltage(adcBASE_t *adc, uint8_t group, uint8_t channel, float *adcValue) {
    if((adc == NULL) || (adcValue == NULL)) {
        return 0;
    }

    adcData_t adcData;
    adcDigitalVoltage(adc, group, channel, &adcData);

    *adcValue = (float) ADC_RESOLUTION/ ((float) (adcData.value)) * 5.00;

    return 1;
}