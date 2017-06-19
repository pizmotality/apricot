/* terminal.h - Defines and function definitions for the virtual terminal
 */

#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"

#define N_TERMINAL  3

typedef struct terminal_t {
    uint16_t status;
    uint16_t nprocess;
    uint32_t esp;
    uint32_t ebp;
    uint32_t return_address;
} terminal_t;

void init_terminal();

int32_t open_terminal();
int32_t close_terminal();
int32_t read_terminal(int32_t fd, int8_t* buf, int32_t nbytes);
int32_t write_terminal(const int8_t* buf, int32_t nbytes);

#endif /* _TERMINAL_H */
