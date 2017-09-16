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

terminal_t* get_current_terminal();

#endif /* _TERMINAL_H */
