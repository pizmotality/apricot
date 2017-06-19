/* terminal.h - Defines and function definitions for the virtual terminal
 */

#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"

#define N_TERMINAL  3

typedef struct terminal_t {
    uint32_t status;
    uint32_t n_process;
    uint32_t esp;
    uint32_t ebp;
    uint32_t return_address;
    uint32_t pid;
    uint32_t reserved[2];
} terminal_t;

void init_terminal();

terminal_t* get_current_terminal();

#endif /* _TERMINAL_H */
