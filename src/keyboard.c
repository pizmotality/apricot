/* keyboard.c - Functions to handle keyboard input
 */

#include "keyboard.h"
#include "i8259.h"
#include "lib.h"

void init_keyboard() {
    ;
}

void handle_keyboard() {
    cli();
    send_eoi(IRQ_KEYBOARD);

    uint8_t key;
    key = inb(KEYBOARD_DATA_PORT);

    sti();
}
