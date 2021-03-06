/* lib.h - Defines for useful library functions
 */

#ifndef _LIB_H
#define _LIB_H

#include "types.h"

void update_cursor(uint32_t tty);

void newline(uint32_t tty);
void backspace(uint32_t tty);
int32_t puts(int8_t* s, uint32_t tty);
void putc(uint8_t c, uint32_t tty);

void kclear();
int32_t kprintf(int8_t* format, ...);
int32_t kputs(int8_t* s);
void kputc(uint8_t c);

int8_t* itoa(uint32_t value, int8_t* buf, int32_t radix);
int8_t* strrev(int8_t* s);
uint32_t strlen(const int8_t* s);
uint32_t strnlen(const int8_t* s, uint32_t n);
uint32_t strlen_tty(const int8_t* s);

void* memset(void* s, int32_t c, uint32_t n);
void* memset_word(void* s, int32_t c, uint32_t n);
void* memset_dword(void* s, int32_t c, uint32_t n);
void* memcpy(void* dest, const void* src, uint32_t n);
void* memmove(void* dest, const void* src, uint32_t n);
int32_t strncmp(const int8_t* s1, const int8_t* s2, uint32_t n);
int8_t* strcpy(int8_t* dest, const int8_t* src);
int8_t* strncpy(int8_t* dest, const int8_t* src, uint32_t n);

/* Port read functions */
/* Inb reads a byte and returns its value as a zero-extended 32-bit
 * unsigned int */
static inline uint32_t inb(uint16_t port) {
    uint32_t val;
    asm volatile("                  \n\
                 xorl   %0, %0      \n\
                 inb    (%w1), %b0  \n\
                 "
                 : "=a"(val)
                 : "d"(port)
                 : "memory");

    return val;
}

/* Reads two bytes from two consecutive ports, starting at "port",
 * concatenates them little-endian style, and returns them zero-extended
 * */
static inline uint32_t inw(uint16_t port) {
    uint32_t val;
    asm volatile("                  \n\
                 xorl   %0, %0      \n\
                 inw    (%w1), %w0  \n\
                 "
                 : "=a"(val)
                 : "d"(port)
                 : "memory");

    return val;
}

/* Reads four bytes from four consecutive ports, starting at "port",
 * concatenates them little-endian style, and returns them */
static inline uint32_t inl(uint16_t port) {
    uint32_t val;
    asm volatile("                  \n\
                 inl    (%w1), %0   \n\
                 "
                 : "=a"(val)
                 : "d"(port)
                 : "memory");

    return val;
}

/* Writes a byte to a port */
#define outb(data, port)                    \
do {                                        \
    asm volatile("outb  %b1, (%w0)"         \
                 :                          \
                 : "d" (port), "a" (data)   \
                 : "memory", "cc");         \
} while (0)

/* Writes two bytes to two consecutive ports */
#define outw(data, port)                    \
do {                                        \
    asm volatile("outw  %w1, (%w0)"         \
                 :                          \
                 : "d" (port), "a" (data)   \
                 : "memory", "cc" );        \
} while (0)

/* Writes four bytes to four consecutive ports */
#define outl(data, port)                    \
do {                                        \
    asm volatile("outl  %l1, (%w0)"         \
                 :                          \
                 : "d" (port), "a" (data)   \
                 : "memory", "cc" );        \
} while (0)

/* Clear interrupt flag - disables interrupts on this processor */
#define cli()                               \
do {                                        \
    asm volatile("cli"                      \
                 :                          \
                 :                          \
                 : "memory", "cc");         \
} while (0)

/* Save flags and then clear interrupt flag
 * Saves the EFLAGS register into the variable "flags", and then
 * disables interrupts on this processor */
#define cli_and_save(flags)                 \
do {                                        \
    asm volatile("                        \n\
                 pushfl                   \n\
                 popl %0                  \n\
                 cli"                       \
                 : "=r"(flags)              \
                 :                          \
                 : "memory", "cc");         \
} while (0)

/* Set interrupt flag - enable interrupts on this processor */
#define sti()                               \
do {                                        \
    asm volatile("sti"                      \
                 :                          \
                 :                          \
                 : "memory", "cc");         \
} while (0)

/* Restore flags
 * Puts the value in "flags" into the EFLAGS register.  Most often used
 * after a cli_and_save_flags(flags) */
#define restore_flags(flags)                \
do {                                        \
    asm volatile("                        \n\
                 pushl  %0                \n\
                 popfl                    \n\
                 "                          \
                 :                          \
                 : "r"(flags)               \
                 : "memory", "cc");         \
} while (0)

#endif /* _LIB_H */
