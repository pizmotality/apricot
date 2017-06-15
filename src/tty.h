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

void init_tty();

void handle_key_event(uint32_t key_event);

#endif /* _TTY_H */
