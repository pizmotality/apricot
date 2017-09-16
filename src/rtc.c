/* rtc.c - Functions for interacting with the RTC
 */

#include "rtc.h"
#include "i8259.h"
#include "lib.h"

void init_rtc() {
    cli();

    outb(RTC_REGISTER_B, RTC_PORT);
    uint8_t prev = inb(RTC_DATA_PORT);
    prev = (prev & 0xFB) | 0x43;
    outb(RTC_REGISTER_B, RTC_PORT);
    outb(prev, RTC_DATA_PORT);

    sti();
}

void handle_rtc() {
    cli();
    send_eoi(IRQ_RTC);

    outb(RTC_REGISTER_C, RTC_PORT);
    inb(RTC_DATA_PORT);

    sti();
}

int32_t open_rtc() {
    return -1;
}

int32_t close_rtc() {
    return -1;
}

int32_t read_rtc(int32_t fd, int8_t* buf, int32_t nbytes) {
    return -1;
}

int32_t write_rtc(const int8_t* buf, int32_t nbytes) {
    return -1;
}

void set_rtc_freq(uint32_t freq) {
    if (freq & (freq - 1))
        return;

    if (freq & 0xFFFFF800)
        return;
}
