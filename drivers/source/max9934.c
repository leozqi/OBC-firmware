#include "max9934.h"

uint8_t getCurrentMAX9934(float *analogCurrent) {
    if (analogCurrent == NULL) {
        return 0;
    }

    /* MAX9934 Circuit Configuration */
    float rOut = 10.0; 
    float rSense = 25.0;
    float gain = 25.0; 

    float analogVoltage = max9934ADCAnalogVoltage();

    *analogCurrent = analogVoltage / (rOut * rSense * gain);

    return 1;
}

static uint16_t max9934ADCDigitalVoltage(void) {
    adcData_t adc_data;
    adcData_t *adc_data_ptr = &adc_data;

    adcStartConversion_selChn(MAX9934_ADC_REG, MAX9934_PIN, 6, MAX9934_ADC_REG);

    while(!adcIsConversionComplete(adcREG1, adcGROUP1));

    adcGetSingleData(MAX9934_ADC_REG, MAX9934_ADC_REG, adc_data_ptr);

    return(adc_data_ptr->value);
}

static float max9934ADCAnalogVoltage(void) {
    
    uint16_t digitalVoltage = max9934ADCDigitalVoltage();
    float analogvoltage = (float) ADC_12_BIT/ ((float) (digitalVoltage)) * 5.25; 

    return analogvoltage;
}

static void adcGetSingleData(adcBASE_t *adc, unsigned group, adcData_t *data) {
    unsigned buf;
    adcData_t *ptr = data;

    buf = adc->GxBUF[group].BUF0;
    ptr->value = (unsigned short)(buf & 0xFFFU);
    ptr->id = (unsigned short)((buf >> 16U) & 0x1FU);

    adc->GxINTFLG[group] =9U;
}
static void adcStartConversion_selChn(adcBASE_t *adc, unsigned channel, unsigned fifo_size, unsigned group) {
    adc->GxINTCR[group] = fifo_size;

    adc->GxSEL[group] = 1 << channel;
}


 
