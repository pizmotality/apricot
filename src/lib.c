/* lib.c - Some basic printing and library functions
 */

#include "lib.h"
#include "tty.h"
#include "memory.h"

#define NUM_COLS    80
#define NUM_ROWS    25
#define ATTRIB      0x7

static int32_t screen_x[NTTY];
static int32_t screen_y[NTTY];

#define CURSOR_LOW  0xF
#define CURSOR_HIGH 0xE

#define VGA_INDEX_REGISTER  0x3D4
#define VGA_DATA_REGISTER   0x3D5

static uint8_t* video_mem = (uint8_t*)VMEM_VIDEO;
static uint8_t* video_mem_persist = (uint8_t*)VMEM_VIDEO_PERSIST;

void update_cursor(uint32_t tty) {
    if (tty != current_tty)
        return;

    uint16_t position = NUM_COLS * screen_y[tty] + screen_x[tty];

    outb(CURSOR_LOW, VGA_INDEX_REGISTER);
    outb((uint8_t)(position & 0xFF), VGA_DATA_REGISTER);
    outb(CURSOR_HIGH, VGA_INDEX_REGISTER);
    outb((uint8_t)((position >> 8) & 0xFF), VGA_DATA_REGISTER);
}

void newline(uint32_t tty) {
    screen_x[tty] = 0;
    if (screen_y[tty] != NUM_ROWS - 1) {
        ++screen_y[tty];
    } else {
        memmove((uint8_t*)video_mem, (uint8_t*)(video_mem + (NUM_COLS << 1)),
            2 * NUM_COLS * (NUM_ROWS - 1) * sizeof(uint8_t));

        int32_t i;
        for (i = 0; i < NUM_COLS; ++i) {
            *(uint8_t*)(video_mem + ((NUM_COLS * (NUM_ROWS - 1) + i) << 1)) = ' ';
            *(uint8_t*)(video_mem + ((NUM_COLS * (NUM_ROWS - 1) + i) << 1) + 1) = ATTRIB;
        }
    }

    update_cursor(tty);
}

void backspace(uint32_t tty) {
    screen_x[tty] = (screen_x[tty] + (NUM_COLS - 1)) % NUM_COLS;
    if (screen_y[tty]) screen_y[tty] -= (screen_x[tty] / (NUM_COLS - 1));
    *(uint8_t*)(video_mem + ((NUM_COLS * screen_y[tty] + screen_x[tty]) << 1)) = ' ';
    *(uint8_t*)(video_mem + ((NUM_COLS * screen_y[tty] + screen_x[tty]) << 1) + 1) = ATTRIB;

    update_cursor(tty);
}

/* Output a string to the selected tty */
int32_t puts(int8_t* s, uint32_t tty) {
    register int32_t index = 0;
    while (s[index] != '\0') {
        putc(s[index], tty);
        index++;
    }

    return index;
}

void putc(uint8_t c, uint32_t tty) {
    if (c == '\n' || c == '\r') {
        newline(tty);
    } else {
        *(uint8_t*)(video_mem + ((NUM_COLS * screen_y[tty] + screen_x[tty]) << 1)) = c;
        *(uint8_t*)(video_mem + ((NUM_COLS * screen_y[tty] + screen_x[tty]) << 1) + 1) = ATTRIB;
        if (++screen_x[tty] == NUM_COLS)
            newline(tty);
    }

    update_cursor(tty);
}

void kclear() {
    screen_x[current_tty] = 0;
    screen_y[current_tty] = 0;
    int32_t i;
    for (i = 0; i < NUM_ROWS * NUM_COLS; i++) {
        *(uint8_t*)(video_mem + (i << 1)) = ' ';
        *(uint8_t*)(video_mem + (i << 1) + 1) = ATTRIB;
    }

    update_cursor(current_tty);
}

/* Standard kprintf().
 * Only supports the following format strings:
 * %%  - print a literal '%' character
 * %x  - print a number in hexadecimal
 * %u  - print a number as an unsigned integer
 * %d  - print a number as a signed integer
 * %c  - print a character
 * %s  - print a string
 * %#x - print a number in 32-bit aligned hexadecimal, i.e.
 *       print 8 hexadecimal digits, zero-padded on the left.
 *       For example, the hex number "E" would be printed as
 *       "0000000E".
 *       Note: This is slightly different than the libc specification
 *       for the "#" modifier (this implementation doesn't add a "0x" at
 *       the beginning).
 *       Also note: %x is the only conversion specifier that can use
 *       the "#" modifier to alter output.
 * */
int32_t kprintf(int8_t* format, ...) {
    /* Pointer to the format string */
    int8_t* buf = format;

    /* Stack pointer for the other parameters */
    int32_t* esp = (void*)&format;
    esp++;

    while (*buf != '\0') {
        switch (*buf) {
            case '%': {
                int32_t alternate = 0;
                buf++;

format_char_switch:
                /* Conversion specifiers */
                switch (*buf) {
                    /* Print a literal '%' character */
                    case '%':
                        kputc('%');
                        break;

                    /* Use alternate formatting */
                    case '#':
                        alternate = 1;
                        buf++;
                        /* Yes, I know gotos are bad.  This is the
                         * most elegant and general way to do this,
                         * IMHO. */
                        goto format_char_switch;

                    /* Print a number in hexadecimal form */
                    case 'x': {
                        int8_t conv_buf[64];
                        if (alternate == 0) {
                            itoa(*((uint32_t*)esp), conv_buf, 16);
                            kputs(conv_buf);
                        } else {
                            int32_t starting_index;
                            int32_t i;
                            itoa(*((uint32_t*)esp), &conv_buf[8], 16);
                            i = starting_index = strlen(&conv_buf[8]);
                            while (i < 8) {
                                conv_buf[i] = '0';
                                i++;
                            }
                            kputs(&conv_buf[starting_index]);
                        }
                        esp++; }
                        break;

                    /* Print a number in unsigned int form */
                    case 'u': {
                        int8_t conv_buf[36];
                        itoa(*((uint32_t*)esp), conv_buf, 10);
                        kputs(conv_buf);
                        esp++; }
                        break;

                    /* Print a number in signed int form */
                    case 'd': {
                        int8_t conv_buf[36];
                        int32_t value = *((int32_t*)esp);
                        if (value < 0) {
                            conv_buf[0] = '-';
                            itoa(-value, &conv_buf[1], 10);
                        } else {
                            itoa(value, conv_buf, 10);
                        }
                        kputs(conv_buf);
                        esp++; }
                        break;

                    /* Print a single character */
                    case 'c':
                        kputc((uint8_t) * ((int32_t*)esp));
                        esp++;
                        break;

                    /* Print a NULL-terminated string */
                    case 's':
                        kputs(*((int8_t**)esp));
                        esp++;
                        break;

                    default:
                        break;
                } }
                break;

            default:
                kputc(*buf);
                break;
        }

        buf++;
    }

    return (buf - format);
}

/* Output a string to the current tty */
int32_t kputs(int8_t* s) {
    register int32_t index = 0;
    while (s[index] != '\0') {
        kputc(s[index]);
        index++;
    }

    return index;
}

void kputc(uint8_t c) {
    if (c == '\n' || c == '\r') {
        newline(current_tty);
    } else {
        *(uint8_t*)(video_mem_persist + ((NUM_COLS * screen_y[current_tty] + screen_x[current_tty]) << 1)) = c;
        *(uint8_t*)(video_mem_persist + ((NUM_COLS * screen_y[current_tty] + screen_x[current_tty]) << 1) + 1) = ATTRIB;
        if (++screen_x[current_tty] == NUM_COLS)
            newline(current_tty);
    }

    update_cursor(current_tty);
}

/* Real library functions */

/* Convert a number to its ASCII representation, with base "radix" */
int8_t* itoa(uint32_t value, int8_t* buf, int32_t radix) {
    static int8_t lookup[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int8_t* newbuf = buf;
    int32_t i;
    uint32_t newval = value;

    /* Special case for zero */
    if (value == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    /* Go through the number one place value at a time, and add the
     * correct digit to "newbuf".  We actually add characters to the
     * ASCII string from lowest place value to highest, which is the
     * opposite of how the number should be printed.  We'll reverse the
     * characters later. */
    while (newval > 0) {
        i = newval % radix;
        *newbuf = lookup[i];
        newbuf++;
        newval /= radix;
    }

    /* Add a terminating NULL */
    *newbuf = '\0';

    /* Reverse the string and return */
    return strrev(buf);
}

/* In-place string reversal */
int8_t* strrev(int8_t* s) {
    register int8_t tmp;
    register int32_t beg = 0;
    register int32_t end = strlen(s) - 1;

    while (beg < end) {
        tmp = s[end];
        s[end] = s[beg];
        s[beg] = tmp;
        beg++;
        end--;
    }

    return s;
}

/* String length */
uint32_t strlen(const int8_t* s) {
    register uint32_t len = 0;
    while (s[len] != '\0')
        len++;

    return len;
}

uint32_t strnlen(const int8_t* s, uint32_t n) {
    register uint32_t len = 0;
    while (s[len] != '\0' && len < n)
        len++;

    return len;
}

uint32_t strlen_tty(const int8_t* s) {
    register uint32_t len = 0;
    while (s[len] != '\0' && s[len] != '\n')
        len++;

    if (s[len] == '\n')
        len++;

    return len;
}

/* Optimized memset */
void* memset(void* s, int32_t c, uint32_t n) {
    c &= 0xFF;
    asm volatile("                          \n\
                 .memset_top:               \n\
                 testl   %%ecx, %%ecx       \n\
                 jz      .memset_done       \n\
                 testl   $0x3, %%edi        \n\
                 jz      .memset_aligned    \n\
                 movb    %%al, (%%edi)      \n\
                 addl    $1, %%edi          \n\
                 subl    $1, %%ecx          \n\
                 jmp     .memset_top        \n\
                 .memset_aligned:           \n\
                 movw    %%ds, %%dx         \n\
                 movw    %%dx, %%es         \n\
                 movl    %%ecx, %%edx       \n\
                 shrl    $2, %%ecx          \n\
                 andl    $0x3, %%edx        \n\
                 cld                        \n\
                 rep     stosl              \n\
                 .memset_bottom:            \n\
                 testl   %%edx, %%edx       \n\
                 jz      .memset_done       \n\
                 movb    %%al, (%%edi)      \n\
                 addl    $1, %%edi          \n\
                 subl    $1, %%edx          \n\
                 jmp     .memset_bottom     \n\
                 .memset_done:              \n\
                 "
                 :
                 : "a"(c << 24 | c << 16 | c << 8 | c), "D"(s), "c"(n)
                 : "edx", "memory", "cc");

    return s;
}

/* Optimized memset_word */
void* memset_word(void* s, int32_t c, uint32_t n) {
    asm volatile("                          \n\
                 movw    %%ds, %%dx         \n\
                 movw    %%dx, %%es         \n\
                 cld                        \n\
                 rep     stosw              \n\
                 "
                 :
                 : "a"(c), "D"(s), "c"(n)
                 : "edx", "memory", "cc");

    return s;
}

/* Optimized memset_dword */
void* memset_dword(void* s, int32_t c, uint32_t n) {
    asm volatile("                          \n\
                 movw    %%ds, %%dx         \n\
                 movw    %%dx, %%es         \n\
                 cld                        \n\
                 rep     stosl              \n\
                 "
                 :
                 : "a"(c), "D"(s), "c"(n)
                 : "edx", "memory", "cc");

    return s;
}

/* Optimized memcpy */
void* memcpy(void* dest, const void* src, uint32_t n) {
    asm volatile("                          \n\
                 .memcpy_top:               \n\
                 testl   %%ecx, %%ecx       \n\
                 jz      .memcpy_done       \n\
                 testl   $0x3, %%edi        \n\
                 jz      .memcpy_aligned    \n\
                 movb    (%%esi), %%al      \n\
                 movb    %%al, (%%edi)      \n\
                 addl    $1, %%edi          \n\
                 addl    $1, %%esi          \n\
                 subl    $1, %%ecx          \n\
                 jmp     .memcpy_top        \n\
                 .memcpy_aligned:           \n\
                 movw    %%ds, %%dx         \n\
                 movw    %%dx, %%es         \n\
                 movl    %%ecx, %%edx       \n\
                 shrl    $2, %%ecx          \n\
                 andl    $0x3, %%edx        \n\
                 cld                        \n\
                 rep     movsl              \n\
                 .memcpy_bottom:            \n\
                 testl   %%edx, %%edx       \n\
                 jz      .memcpy_done       \n\
                 movb    (%%esi), %%al      \n\
                 movb    %%al, (%%edi)      \n\
                 addl    $1, %%edi          \n\
                 addl    $1, %%esi          \n\
                 subl    $1, %%edx          \n\
                 jmp     .memcpy_bottom     \n\
                 .memcpy_done:              \n\
                 "
                 :
                 : "S"(src), "D"(dest), "c"(n)
                 : "eax", "edx", "memory", "cc");

    return dest;
}

/* Optimized memmove (used for overlapping memory areas) */
void* memmove(void* dest, const void* src, uint32_t n) {
    asm volatile("                                  \n\
                 movw    %%ds, %%dx                 \n\
                 movw    %%dx, %%es                 \n\
                 cld                                \n\
                 cmp     %%edi, %%esi               \n\
                 jae     .memmove_go                \n\
                 leal    -1(%%esi, %%ecx), %%esi    \n\
                 leal    -1(%%edi, %%ecx), %%edi    \n\
                 std                                \n\
                 .memmove_go:                       \n\
                 rep     movsb                      \n\
                 "
                 :
                 : "D"(dest), "S"(src), "c"(n)
                 : "edx", "memory", "cc");

    return dest;
}

/* Standard strncmp */
int32_t strncmp(const int8_t* s1, const int8_t* s2, uint32_t n) {
    int32_t i;
    for (i = 0; i < n; i++) {
        if ((s1[i] != s2[i]) || (s1[i] == '\0')) {
            return s1[i] - s2[i];
        }
    }
    return 0;
}

/* Standard strcpy */
int8_t* strcpy(int8_t* dest, const int8_t* src) {
    int32_t i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';
    return dest;
}

/* Standard strncpy */
int8_t* strncpy(int8_t* dest, const int8_t* src, uint32_t n) {
    int32_t i = 0;
    while (src[i] != '\0' && i < n) {
        dest[i] = src[i];
        i++;
    }

    while (i < n) {
        dest[i] = '\0';
        i++;
    }

    return dest;
}

void test_interrupts(void) {
    int32_t i;
    for (i = 0; i < NUM_ROWS * NUM_COLS; i++) {
        video_mem[i << 1]++;
    }
}
