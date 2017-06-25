/* rtc.h - Defines and function definitions for interacting with the RTC
 */

#ifndef _RTC_H
#define _RTC_H

#include "types.h"

#define RTC_PORT        0x70
#define RTC_DATA_PORT   0x71

#define RTC_CR_A    0x8A
#define RTC_CR_B    0x8B
#define RTC_CR_C    0x8C

typedef struct time_t {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t century;
} time_t;

#define RTCCLOSE    0x0
#define RTCOPEN     0x1
#define RTCSET      0x2

void init_rtc();

void handle_rtc();

int32_t read_rtc(int32_t fd, int8_t* buf, int32_t nbytes);
int32_t write_rtc(const int8_t* buf, int32_t nbytes);
int32_t open_rtc();
int32_t close_rtc();

int32_t set_rtc_freq(uint32_t freq);

#endif /* _RTC_H */
