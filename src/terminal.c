/* terminal.c - Functions to interact with the terminal
 */

#include "terminal.h"
#include "process.h"

terminal_t terminal[N_TERMINAL];

uint32_t current_terminal;

void init_terminal() {
    uint32_t i;
    for (i = 0; i < N_TERMINAL; ++i) {
        terminal[i].status = 0;
        terminal[i].nprocess = 0;
    }

    current_terminal = 0;
}

int32_t open_terminal() {
    return -1;
}

int32_t close_terminal() {
    return -1;
}

int32_t read_terminal(int32_t fd, int8_t* buf, int32_t nbytes) {
    return -1;
}

int32_t write_terminal(const int8_t* buf, int32_t nbytes) {
    return -1;
}
