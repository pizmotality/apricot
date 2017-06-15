/* keyboard.c - Functions to handle keyboard input
 */

#include "keyboard.h"
#include "i8259.h"
#include "tty.h"
#include "lib.h"

void init_keyboard() {
    init_tty();
}

void handle_keyboard() {
    cli();
    send_eoi(IRQ_KEYBOARD);

    uint8_t scan_code;
    scan_code = inb(KEYBOARD_DATA_PORT);

    uint32_t key_event;
    if (scan_code == SCAN_CODE_SEQUENCE) {
        uint8_t scan_code_next;
        scan_code_next = inb(KEYBOARD_DATA_PORT);
        key_event = (scan_code << 8) | scan_code_next;
    } else {
        key_event = scan_code;
    }

    handle_key_event(key_event);

    sti();
}
