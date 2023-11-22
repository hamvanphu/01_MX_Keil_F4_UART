#ifndef __DS3231_H
#define __DS3231_H

#include "main.h"

uint8_t Decimal2BCD(uint8_t num);
uint8_t BCD2Decimal(uint8_t num);
uint8_t rtc_get_day_of_week(uint8_t date, uint8_t mon, uint8_t year);
void rtc_read_time(uint8_t *hour, uint8_t *min, uint8_t *sec);
void rtc_read_date(uint8_t *date, uint8_t *mon, uint8_t *year);
void rtc_write_time(uint8_t hour, uint8_t min, uint8_t sec);
void rtc_write_date(uint8_t date, uint8_t mon, uint8_t year);
float rtc_read_temp(void);
void rtc_init(I2C_HandleTypeDef *hi2c);

#endif // __DS3231_H
