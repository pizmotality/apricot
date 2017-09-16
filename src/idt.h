/* idt.h - Defines used in setting up the IDT
 */

#ifndef _IDT_H
#define _IDT_H

#define INT_PIT         0x20
#define INT_KEYBOARD    0x21
#define INT_RTC         0x28
#define INT_SYSTEM_CALL 0x80

void init_idt();

#endif /* _IDT_H */
