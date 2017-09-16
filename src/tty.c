/* tty.c - Functions for TTY and line discipline
 */

#include "tty.h"
#include "signal.h"
#include "process.h"
#include "memory.h"
#include "page.h"
#include "lib.h"

#define PMEM_VIDEO_BUFFER   (PMEM_VIDEO + MEM_PAGE)

static int8_t line_buffer[LINE_BUFFER_SIZE];
static uint32_t line_buffer_index;

static inline void clear_line_buffer() __attribute__((always_inline));
static inline void clear_line_buffer() {
    memset(line_buffer, '\0', LINE_BUFFER_SIZE * sizeof(uint8_t));
    line_buffer_index = 0;
}

static uint8_t key_event_char[0x80] = {
    0x00, 0x00, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x30, 0x2D, 0x3D, 0x00, 0x00,
    0x71, 0x77, 0x65, 0x72, 0x74, 0x79, 0x75, 0x69,
    0x6F, 0x70, 0x5B, 0x5D, 0x0A, 0x00, 0x61, 0x73,
    0x64, 0x66, 0x67, 0x68, 0x6A, 0x6B, 0x6C, 0x3B,
    0x27, 0x60, 0x00, 0x5C, 0x7A, 0x78, 0x63, 0x76,
    0x62, 0x6E, 0x6D, 0x2C, 0x2E, 0x2F, 0x00, 0x00,
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x21, 0x40, 0x23, 0x24, 0x25, 0x5E,
    0x26, 0x2A, 0x28, 0x29, 0x5F, 0x2B, 0x00, 0x00,
    0x51, 0x57, 0x45, 0x52, 0x54, 0x59, 0x55, 0x49,
    0x4F, 0x50, 0x7B, 0x7D, 0x00, 0x00, 0x41, 0x53,
    0x44, 0x46, 0x47, 0x48, 0x4A, 0x4B, 0x4C, 0x3A,
    0x22, 0x7E, 0x00, 0x7C, 0x5A, 0x58, 0x43, 0x56,
    0x42, 0x4E, 0x4D, 0x3C, 0x3E, 0x3F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint32_t flag_ctrl;
static uint32_t flag_alt;
static uint32_t flag_shift;
static uint32_t flag_caps;

tty_t ttys[NTTY];
uint32_t current_tty;

void init_tty() {
    clear_line_buffer();

    flag_ctrl = 0;
    flag_alt = 0;
    flag_shift = 0;
    flag_caps = 0;

    uint32_t i;
    for (i = 0; i < NTTY; ++i)
        ttys[i].flags = 0;

    current_tty = 0;
}

void switch_tty(uint32_t target) {
    if (target == current_tty)
        return;

    cli();
    disable_paging();

    memcpy((uint8_t*)(PMEM_VIDEO_BUFFER + current_tty * MEM_PAGE), (uint8_t*)PMEM_VIDEO, MEM_PAGE);
    memcpy((uint8_t*)PMEM_VIDEO, (uint8_t*)(PMEM_VIDEO_BUFFER + target * MEM_PAGE), MEM_PAGE);

    if (current_tty == current_process->tty)
        map_memory_page(VMEM_VIDEO, PMEM_VIDEO_BUFFER + current_process->tty * MEM_PAGE, SUPERVISOR, page_table);
    if (target == current_process->tty)
        map_memory_page(VMEM_VIDEO, PMEM_VIDEO, SUPERVISOR, page_table);
    /* remap VMEM_VIDEO_USER if mapped */
    /* list of mapped user pages per process is required */

    enable_paging();
    sti();

    current_tty = target;
}

void handle_key_event(uint32_t key_event) {
    switch (key_event) {
        case KEY_PRESS_LEFT_CTRL:
        case KEY_PRESS_RIGHT_CTRL:
            ++flag_ctrl;
            break;
        case KEY_RELEASE_LEFT_CTRL:
        case KEY_RELEASE_RIGHT_CTRL:
            --flag_ctrl;
            break;
        case KEY_PRESS_LEFT_ALT:
        case KEY_PRESS_RIGHT_ALT:
            ++flag_alt;
            break;
        case KEY_RELEASE_LEFT_ALT:
        case KEY_RELEASE_RIGHT_ALT:
            --flag_alt;
            break;
        case KEY_PRESS_LEFT_SHIFT:
        case KEY_PRESS_RIGHT_SHIFT:
            ++flag_shift;
            break;
        case KEY_RELEASE_LEFT_SHIFT:
        case KEY_RELEASE_RIGHT_SHIFT:
            --flag_shift;
            break;
        case KEY_PRESS_CAPS:
            flag_caps ^= 0x1;
            break;
        case KEY_PRESS_ENTER:
            ttys[current_tty].flags |= 0x1;
            break;
        case KEY_PRESS_BACKSPACE:
            if (line_buffer_index) {
                line_buffer[--line_buffer_index] = '\0';
                backspace();
            }
            break;
        case KEY_PRESS_C:
            if (flag_ctrl) {
                queue_signal(SIGINT);
                return;
            }
            break;
        case KEY_PRESS_1:
        case KEY_PRESS_2:
        case KEY_PRESS_3:
            if (flag_ctrl) {
                switch_tty(key_event - TTY_OFFSET);
                return;
            }
            break;
    }

    if (key_event < 0x40 && key_event_char[key_event]) {
        if (line_buffer_index == LINE_BUFFER_SIZE) {
            ttys[current_tty].flags |= 0x1;
            newline();
        }

        if (flag_shift)
            key_event += 0x40;
        line_buffer[line_buffer_index] = key_event_char[key_event];
        putc(line_buffer[line_buffer_index++]);
    }
}

int32_t read_tty(int32_t fd, int8_t* buf, int32_t nbytes) {
    while (!(ttys[current_tty].flags & 0x1));

    uint32_t length = strlen_tty(line_buffer);
    if (length > nbytes)
        length = nbytes;

    strncpy(buf, line_buffer, length);

    cli();
    memmove(line_buffer, line_buffer + length, LINE_BUFFER_SIZE - length);
    memset(line_buffer + LINE_BUFFER_SIZE - length, '\0', length);
    line_buffer_index -= length;
    sti();

    ttys[current_tty].flags &= 0xFFFFFFFE;

    return length;
}

int32_t write_tty(const int8_t* buf, int32_t nbytes) {
    uint32_t length = nbytes;
    if (strlen(buf) < nbytes)
        length = strlen(buf);

    uint32_t i;
    for (i = 0; i < length; ++i)
        putc(buf[i]);

    return length;
}

int32_t open_tty() {
    return 0;
}

int32_t close_tty() {
    return 0;
}
