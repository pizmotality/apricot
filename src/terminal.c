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

    init_process();
}

terminal_t* get_current_terminal() {
    return (terminal_t*)terminal + current_terminal;
}
