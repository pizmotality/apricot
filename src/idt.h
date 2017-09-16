/* idt.h - Defines used in setting up the IDT
 */

#ifndef _IDT_H
#define _IDT_H

#define IRQ_PIT         0x20
#define IRQ_KEYBOARD    0x21
#define IRQ_RTC         0x28
#define IRQ_SYSCALL     0x80

void init_idt();

#endif /* _IDT_H */
