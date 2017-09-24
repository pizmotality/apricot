/* pit.c - Functions to initialize PIT
 */

#include "pit.h"
#include "schedule.h"
#include "i8259.h"
#include "lib.h"

void init_pit() {
    outb(PIT_MODE, MODE_COMMAND_REGISTER);
    outb(PIT_FREQ & 0xFF, CHANNEL0);
    outb(PIT_FREQ >> 8, CHANNEL0);
}

void handle_pit() {
    cli();
    send_eoi(IRQ_PIT);

    sti();

    int32_t pid = sched_next();
    if (pid != -1)
        raise(pid);
}
