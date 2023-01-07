#include "ds3232_mz.h"
#include "obc_i2c_io.h"

#include <stdint.h>

const uint8_t LOW_BIT_MASK = 0b00001111;
const uint8_t HIGH_BIT_MASK = 0b01110000;

void resetRTC(void) {
    /* GIO pin output in the rm46 is the input for the RST pin the RTC. Setting GIOApin1 to 0
    means that the open drain circuit in the RTC will be high impedence (no current flows). So
    nothing will happen. Setting it high will pull the open draain circuit to ground making it 0
    which resets the RTC. */ 
    gioSetBit(gioPORTA, 1, 1);
    gioSetBit(gioPORTA, 1, 0);
}

uint8_t getSecondsRTC(uint8_t *seconds) {
    uint8_t data;

    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_SECONDS, &data, 1) == 0) {
        // sciPrintText( (uint8_t*)"Failed to read seconds data\r\n", 35);
        return 0;
    }

    uint8_t oneSeconds = data & LOW_BIT_MASK;
    uint8_t ten_seconds = (data & HIGH_BIT_MASK) >> 4;

    *seconds = ten_seconds*10 + oneSeconds;

    return 1;
}

uint8_t getMinutesRTC(uint8_t *minutes) {
    uint8_t data;

    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_MINUTES, &data, 1) == 0) {
        // sciPrintText( (uint8_t*)"Failed to read minutes data\r\n", 35);
        return 0;
    }
    uint8_t one_minutes = data & LOW_BIT_MASK;
    uint8_t ten_minutes = (data & HIGH_BIT_MASK) >> 4;

    *minutes = ten_minutes*10 + one_minutes;

    return 1;
}

uint8_t getHourRTC(uint8_t *hours){
    uint8_t data;
    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_HOURS, &data, 1) == 0) {
        // sciPrintText( (uint8_t*)"Failed to read hours data\r\n", 35);
        return 0;
    }

    uint8_t one_hours = data & LOW_BIT_MASK;
    uint8_t ten_hours = (data & 48) >> 4;

    *hours = ten_hours*10 + one_hours;
    return 1;

} 

uint8_t getDayRTC(uint8_t *days) {
    uint8_t data;

    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_DAY, &data, 1) == 0) {
        // sciPrintText( (uint8_t*)"Failed to read days data\r\n", 35);
        return 0;
    }
    
    *days = data;

    return 1;
}

uint8_t getDateRTC(uint8_t* date) {
    uint8_t data;

    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_DATE, &data, 1) == 0) {
        // sciPrintText( (uint8_t*)"Failed to read date data\r\n", 35);
        return 0;
    }

    uint8_t ones_date = data & LOW_BIT_MASK;
    uint8_t tens_date = (data & 48) >> 4;   // 48 = 0011 0000
    *date = tens_date*10 + ones_date;

    return 1;
}

uint8_t getMonthRTC(uint8_t *month) {
    uint8_t data;

    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_MONTH, &data, 1) == 0) {
        // sciPrintText( (uint8_t*)"Failed to read month data\r\n", 35);
        return 0;
    }
    
    uint8_t ones_month = data & LOW_BIT_MASK;
    uint8_t tens_month = (data & 16) >> 4; 
    *month = tens_month*10 + ones_month;

    return 1;
}

uint8_t getYearRTC(uint8_t* year) {
    uint8_t data;

    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_YEAR, &data, 1) == 0) {
        // sciPrintText( (uint8_t*)"Failed to read years data\r\n", 35);
        return 0;
    }
    
    uint8_t ones_year = data & LOW_BIT_MASK;
    uint8_t tens_year = (data & HIGH_BIT_MASK) >> 4;

    *year = tens_year*10 + ones_year;

    return 1;
}

uint8_t getCurrentTimeRTC(rtc_time_t *time) {
    uint8_t result = 0;

    if (getMinutesRTC(&(time->minutes))) 
        result = 1;
    else
        result = 0;

    if (getSecondsRTC(&(time->seconds))) 
        result = 1;
    else
        result = 0;

    if (getHourRTC(&(time->hours))) 
        result = 1;
    else
        result = 0;

    return result;

}

uint8_t getCurrentDateTimeRTC(rtc_date_time_t *dateTime) {
    uint8_t result = 0;

    if (getDateRTC(&(dateTime->date))) 
        result = 1;
    else
        result = 0;

    if (getMonthRTC(&(dateTime->month))) 
        result = 1;
    else
        result = 0;

    if (getYearRTC(&(dateTime->year))) 
        result = 1;
    else
        result = 0;

    if (getHourRTC(&(dateTime->hours))) 
        result = 1;
    else
        result = 0;

    if (getMinutesRTC(&(dateTime->minutes))) 
        result = 1;
    else
        result = 0;

    if (getSecondsRTC(&(dateTime->seconds))) 
        result = 1;
    else
        result = 0;

    return result;
}

uint8_t getAlarmTimeRTC(rtc_alarm_time_t *alarmTime) {
    uint8_t seconds;

    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_ALARM_1_SECONDS, &seconds, 1) == 0) {
        return 0;
    }
    alarmTime->seconds = (seconds & LOW_BIT_MASK) | (seconds & HIGH_BIT_MASK);

    uint8_t minutes;
    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_ALARM_1_MINUTES, &minutes, 1) == 0) {
        return 0;
    }
    alarmTime->minutes = (minutes & LOW_BIT_MASK) | (minutes & HIGH_BIT_MASK);

    uint8_t hours;
    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_ALARM_1_HOURS, &hours, 1) == 0) {
        return 0;
    }
    alarmTime->hours = (hours & LOW_BIT_MASK) | (hours & 48);

    uint8_t dayOrDates;
    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_ALARM_1_DAY_OR_DATE, &dayOrDates, 1) == 0) {
        return 0;
    }

    // True if day, false if date
    if (dayOrDates & 64U)
        alarmTime->day = (dayOrDates & LOW_BIT_MASK);
    else
        alarmTime->date = (dayOrDates & LOW_BIT_MASK) + ((dayOrDates & 0b110000) >> 4) * 10;
    
    return 1;
}

uint8_t getControlRTC(rtc_control_t *control) {
    uint8_t data;
    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_CONTROL, &data, 1) == 0) {
        // sciPrintText( (uint8_t*)"Failed to read control data\r\n", 35);
        return 0;
    }

    control->EOSC = (data & 128) >> 7;
    control->BBSQW = (data & 64) >> 6;
    control->CONV = (data & 32) >> 5;
    control->INTCN = (data & 4) >> 2;
    control->A2IE = (data & 2) >> 1;
    control->A1IE = (data & 1);

    return 1;
}

uint8_t getStatusRTC(rtc_status_t *status) {
    uint8_t data;

    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_STATUS, &data, 1) == 0) {
        // sciPrintText( (uint8_t*)"Failed to read status data\r\n", 35);
        return 0;
    }

    status->OSF = (data & 128) >> 7;
    status->BB32KHZ = (data & 64) >> 6;
    status->EN32KHZ = (data & 8) >> 3;
    status->BSY = (data & 4) >> 2;
    status->A2F = (data & 2) >> 1;
    status->A1F = (data & 1);

    return 1;
}

uint8_t getAgingOffsetRTC(int8_t* agingOffset) {
    uint8_t data;
    
    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_AGING, &data, 1) == 0) {
        // sciPrintText( (uint8_t*)"Failed to read aging offset data\r\n", 35);
        return 0;
    }

    *agingOffset = data;
    return 1;
}

float getTemperatureRTC(float* temperature) {
    uint8_t dataLSB, dataMSB;

    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_TEMP_MSB, &dataMSB, 1) == 0) {
        // sciPrintText( (uint8_t*)"Failed to read temperature MSB data\r\n", 35);
        return 0;
    }

    if (i2cReadReg(DS3232_I2C_ADDRESS, DS3232_TEMP_LSB, &dataLSB, 1) == 0) {
        // sciPrintText( (uint8_t*)"Failed to read temperature LSB data\r\n", 35);
        return 0;
    }

    int8_t tempDecimal = dataMSB;

    uint8_t tempFraction = dataLSB;
    int16_t tempCombined = (tempDecimal<<2) | (tempFraction>>6);
    *temperature = tempCombined * 0.25;

    return 1;
}


uint8_t setSecondsRTC(uint8_t writeSeconds) {
    uint8_t writeVal =  combineWriteVal(writeSeconds);

    if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_SECONDS, &writeVal, 1))
        return 0;

    return 1;
}

uint8_t setMinutesRTC(uint8_t writeMinutes) {
    uint8_t writeVal =  combineWriteVal(writeMinutes);

    if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_MINUTES, &writeVal, 1))
        return 0;

    return 1;
}

uint8_t setHourRTC(uint8_t writeHours) {
    // DEFAULT setting hour to 24 hour mode
    uint8_t writeVal = HOUR_MODE | combineWriteVal(writeHours);

    if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_HOURS, &writeVal, 1))
        return 0;
    
    return 1;
}

uint8_t setDayRTC(uint8_t writeDays) {
    if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_DAY, &writeDays, 1))
        return 0;
    
    return 1;
}

uint8_t setDateRTC(uint8_t writeDates) {
    uint8_t writeVal =  combineWriteVal(writeDates);
    
    if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_DATE, &writeVal, 1))
        return 0;
    
    return 1;
}

uint8_t setMonthRTC(uint8_t writeMonths) {
    uint8_t writeVal =  combineWriteVal(writeMonths);

    if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_MONTH, &writeVal, 1))
        return 0;
    
    return 1;
}

uint8_t setYearRTC(uint8_t writeYears) {
    uint8_t writeVal =  combineWriteVal(writeYears);

    if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_YEAR, &writeVal, 1))
        return 0;
    
    return 1;
}


/* writeSeconds, writeminutes and so on written on the same address as A1m1, a2m2 etc. Thus in my implementation, 
I have combined seconds value with A1m1, minutes value with a2m2 before writing it to the correct address. Something
similar with day or date mode */

uint8_t setAlarmRTC(rtc_alarm_time_t *writeAlarmTime, rtc_alarm_mode_t *writeAlarmMode,  uint8_t dayOrDate) {
    uint8_t writeSeconds = combineWriteVal(writeAlarmTime->seconds) | writeAlarmMode-> A1M1;
    if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_ALARM_1_SECONDS, &writeSeconds, 1)) {
        return 0;
    }

    uint8_t writeMinutes =  combineWriteVal(writeAlarmTime->minutes) | writeAlarmMode-> A1M2;
    if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_ALARM_1_MINUTES, &writeMinutes, 1)) {
        return 0;
    }

    // DEFAULT setting hour to 24 hour mode
    uint8_t writeHours = HOUR_MODE | combineWriteVal(writeAlarmTime->hours) | writeAlarmMode-> A1M3;
    if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_HOURS, &writeHours, 1)) {
        return 0;
    }

    // if dayOrDate is 1, its in day mode else date mode
    if (dayOrDate) {
        uint8_t writeDay =  combineWriteVal(writeAlarmTime->day) | writeAlarmMode-> A1M4 | DAY_MODE;
        if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_DATE, &writeDay, 1)) {
          return 0;
        }
    }
    else {
        uint8_t writeDate =  combineWriteVal(writeAlarmTime->date) | writeAlarmMode-> A1M4 | DATE_MODE;
        if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_DATE, &writeDate, 1)) {
          return 0;
        }
    }

    return 1;
}

uint8_t setControlRTC(rtc_control_t *writeControl) {
   uint8_t writeVal =  (writeControl->EOSC << 7) |
                        (writeControl->BBSQW << 6) |
                        (writeControl->CONV << 5) |
                        (writeControl->INTCN << 2) |
                        (writeControl->A2IE << 1) |
                        (writeControl->A1IE);

    if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_CONTROL, &writeVal, 1))
        return 0;
    
    return 1;
}

uint8_t setStatusRTC(rtc_status_t *writeStatus) {
    uint8_t writeVal = (writeStatus->OSF << 7) |
                        (writeStatus->BB32KHZ << 6) |
                        (writeStatus->EN32KHZ << 3) |
                        (writeStatus->BSY << 2) |
                        (writeStatus->A2F << 1) |
                        (writeStatus->A1F);

    if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_STATUS, &writeVal, 1))
        return 0;
    
    return 1;
}

uint8_t setAgingOffsetRTC(int8_t writeAgingOffset) {
    uint8_t writeVal = (writeAgingOffset < 0) ? (1 << 7) : 0;
    writeVal |= (writeAgingOffset < 0) ? -writeAgingOffset : writeAgingOffset;

    if (!i2cWriteReg(DS3232_I2C_ADDRESS, DS3232_AGING, &writeVal, 1))
        return 0;
    
    return 1;
}


uint8_t combineWriteVal(uint8_t inputVal) {
    uint8_t onesdigit = inputVal % 10;
    uint8_t tensdigit = (inputVal / 10) << 4;
    uint8_t writeVal = onesdigit | tensdigit;

    return writeVal;
}