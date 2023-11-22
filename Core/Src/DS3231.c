#include "DS3231.h"
I2C_HandleTypeDef *rtc_hi2c;

uint8_t Decimal2BCD(uint8_t num)
{
  return ((num/10)<<4) | ((num%10) & 0x0F);
}

uint8_t BCD2Decimal(uint8_t num)
{
  return (num>>4)*10 + (num & 0x0F);
}

uint8_t rtc_get_day_of_week(uint8_t date, uint8_t mon, uint8_t year)
{
  return (date += mon < 3 ? year-- : year - 2, 23*mon/9 + date + 4 + year/4 - year/100 + year/400) % 7; 
}

void rtc_read_time(uint8_t *hour, uint8_t *min, uint8_t *sec)
{
  uint8_t buff[3];
  HAL_I2C_Mem_Read(rtc_hi2c, 0x68<<1, 0x00, I2C_MEMADD_SIZE_8BIT, buff, 3, 100);

  *hour = BCD2Decimal(buff[2]);
  *min = BCD2Decimal(buff[1]);
  *sec = BCD2Decimal(buff[0]);
}
void rtc_read_date(uint8_t *date, uint8_t *mon, uint8_t *year)
{
  uint8_t buff[3];
  HAL_I2C_Mem_Read(rtc_hi2c, 0x68<<1, 0x04, I2C_MEMADD_SIZE_8BIT, buff, 3, 100);

  *year = BCD2Decimal(buff[2]);
  *mon = BCD2Decimal(buff[1]);
  *date = BCD2Decimal(buff[0]);
}
void rtc_write_time(uint8_t hour, uint8_t min, uint8_t sec)
{
  uint8_t buff[3]={Decimal2BCD(sec),
                   Decimal2BCD(min),
                   Decimal2BCD(hour)};
  HAL_I2C_Mem_Write(rtc_hi2c, 0x68<<1, 0x00, I2C_MEMADD_SIZE_8BIT, buff, 3, 100);
}
void rtc_write_date(uint8_t date, uint8_t mon, uint8_t year)
{
  uint8_t buff[3]={Decimal2BCD(date),
                   Decimal2BCD(mon),
                   Decimal2BCD(year)};
  HAL_I2C_Mem_Write(rtc_hi2c, 0x68<<1, 0x04, I2C_MEMADD_SIZE_8BIT, buff, 3, 100);
}
float rtc_read_temp(void)
{
  uint8_t buff[2];
  HAL_I2C_Mem_Read(rtc_hi2c, 0x68<<1, 0x011, I2C_MEMADD_SIZE_8BIT, buff, 2, 100);
  float temp = buff[0] + (buff[1]>>6)*0.25;
  if(buff[0] & (1<<7)) // negative temp
  {
    temp *= -1;
  }
  return temp;
}
void rtc_init(I2C_HandleTypeDef *hi2c)
{
    rtc_hi2c = hi2c;
}