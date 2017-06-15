/* rtc.c - Functions for interacting with the RTC
 */

#include "rtc.h"
#include "i8259.h"
#include "lib.h"

void init_rtc() {
    cli();

    outb(RTC_REGISTER_B, RTC_PORT);
    uint8_t prev;
    prev = inb(RTC_DATA_PORT);
    prev &= 0xFB;
    prev |= 0x43;
    outb(RTC_REGISTER_B, RTC_PORT);
    outb(prev, RTC_DATA_PORT);

    sti();
}

void handle_rtc() {
    cli();
    send_eoi(IRQ_RTC);

    outb(RTC_REGISTER_C, RTC_PORT);
    inb(RTC_DATA_PORT);

    printf("RTC INT\n");

    sti();
}
