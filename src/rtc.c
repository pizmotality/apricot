/* rtc.c - Functions for interacting with the RTC
 */

#include "rtc.h"
#include "signal.h"
#include "i8259.h"
#include "lib.h"

int32_t status;

uint32_t frequency;
uint32_t count;

void init_rtc() {
    cli();

    uint8_t prev;
    outb(RTC_CR_B, RTC_PORT);
    prev = inb(RTC_DATA_PORT);
    outb(RTC_CR_B, RTC_PORT);
    outb(prev | 0x40, RTC_DATA_PORT);

    outb(RTC_CR_A, RTC_PORT);
    prev = inb(RTC_DATA_PORT);
    outb(RTC_CR_A, RTC_PORT);
    outb((prev & 0xF0) | 0x0F, RTC_DATA_PORT);

    sti();

    status = RTCCLOSE;
    frequency = get_rtc_freq();
    count = 0;
}

void handle_rtc() {
    cli();
    send_eoi(IRQ_RTC);

    outb(RTC_CR_C, RTC_PORT);
    inb(RTC_DATA_PORT);

    status &= ~RTCSET;

    sti();

    count = count + BASE_FREQ / frequency;
    if (count >= SIGALARM_FREQ) {
        queue_signal(SIGALARM);
        count = count % BASE_FREQ;
    }
}

int32_t read_rtc(int32_t fd, int8_t* buf, int32_t nbytes) {
    status |= RTCSET;
    while (status & RTCSET);

    return 0;
}

int32_t write_rtc(const int8_t* buf, int32_t nbytes) {
    uint32_t freq = *(uint32_t*)buf;
    return set_rtc_freq(freq);
}

int32_t open_rtc() {
    if (status)
        return -1;

    status = RTCOPEN;
    return 0;
}

int32_t close_rtc() {
    if (!status)
        return -1;

    status = RTCCLOSE;
    return 0;
}

int32_t set_rtc_freq(uint32_t freq) {
    if (freq & (freq - 1))
        return -1;

    if (freq & 0xFFFFF800)
        return -1;

    uint32_t divider = 0xF;
    for (; divider > 0x5; --divider, freq = freq >> 1) {
        if (freq & 0x2) {
            cli();

            outb(RTC_CR_A, RTC_PORT);
            uint8_t prev = inb(RTC_DATA_PORT);
            outb(RTC_CR_A, RTC_PORT);
            outb((prev & 0xF0) | divider, RTC_DATA_PORT);

            sti();

            frequency = freq;
            return 0;
        }
    }

    return -1;
}

uint32_t get_rtc_freq() {
    cli();

    outb(RTC_CR_A, RTC_PORT);
    uint8_t prev = inb(RTC_DATA_PORT);
    prev = prev & 0x0F;

    sti();

    return (BASE_FREQ << 1) >> prev;
}
