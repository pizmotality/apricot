/* keyboard.h - Defines and function definitions for handling keyboard input
 */

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define KEYBOARD_COMMAND_PORT   0x64
#define KEYBOARD_DATA_PORT      0x60

#define SCAN_CODE_SEQUENCE  0x30

void init_keyboard();

void handle_keyboard();

#endif /* _KEYBOARD_H */
