/* tty.h - Defines and function definitions for TTY and line discipline
 */

#ifndef _TTY_H
#define _TTY_H

#include "types.h"

#define LINE_BUFFER_SIZE    256

#define KEY_PRESS_LEFT_CTRL         0x1D
#define KEY_RELEASE_LEFT_CTRL       0x9D
#define KEY_PRESS_RIGHT_CTRL        0xE01D
#define KEY_RELEASE_RIGHT_CTRL      0xE09D

#define KEY_PRESS_LEFT_SHIFT        0x2A
#define KEY_RELEASE_LEFT_SHIFT      0xAA
#define KEY_PRESS_RIGHT_SHIFT       0x36
#define KEY_RELEASE_RIGHT_SHIFT     0xB6

#define KEY_PRESS_CAPS              0x3A

#define KEY_PRESS_ENTER             0x1C
#define KEY_PRESS_BACKSPACE         0x0E

#define NTTY    3

typedef struct tty_t {
    uint32_t flags;
} tty_t;

void init_tty();

void handle_key_event(uint32_t key_event);

int32_t read_tty(int32_t fd, int8_t* buf, int32_t nbytes);
int32_t write_tty(const int8_t* buf, int32_t nbytes);
int32_t open_tty();
int32_t close_tty();

#endif /* _TTY_H */
