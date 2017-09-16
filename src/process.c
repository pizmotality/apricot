/* process.c - Functions for controlling processes
 */

#include "process.h"
#include "memory.h"

pcb_t* process_control_block[NPROCESS];
pcb_t* current_process;

void init_process() {
    uint32_t i;
    for (i = 0; i < NPROCESS; ++i) {
        process_control_block[i] = (pcb_t*)(KERNEL_VMEM_BASE - STACK_SIZE * (i + 1));
        process_control_block[i]->state = 0;
    }

    current_process = 0;
}

int32_t get_available_pid() {
    int32_t pid;
    for (pid = 0; pid < NPROCESS; ++pid) {
        if (!process_control_block[pid]->state)
            break;
    }

    if (pid == NPROCESS)
        return -1;

    return pid;
}

pcb_t* get_current_process() {
    return current_process;
}

pcb_t* get_process(int32_t pid) {
    return process_control_block[pid];
}

void set_current_process(int32_t pid) {
    current_process = process_control_block[pid];
}
