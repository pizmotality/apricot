/* rtc.h - Defines and function definitions for interacting with the RTC
 */

#ifndef _RTC_H
#define _RTC_H

#include "types.h"

#define RTC_PORT        0x70
#define RTC_DATA_PORT   0x71

#define RTC_REGISTER_A  0x0A
#define RTC_REGISTER_B  0x0B
#define RTC_REGISTER_C  0x0C

typedef struct time_t {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t century;
} time_t;

void init_rtc();

void handle_rtc();

#endif /* _RTC_H */
