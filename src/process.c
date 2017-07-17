/* process.c - Functions for controlling processes
 */

#include "process.h"
#include "tty.h"
#include "rtc.h"
#include "filesystem.h"
#include "memory.h"

fops_t stdin;
fops_t stdout;
fops_t fops_rtc;
fops_t fops_dir;
fops_t fops_file;

#define _SET_FOPS(FTYPE) {                  \
    fops_##FTYPE.read = &read_##FTYPE;      \
    fops_##FTYPE.write = &write_##FTYPE;    \
    fops_##FTYPE.open = &open_##FTYPE;      \
    fops_##FTYPE.close = &close_##FTYPE;    \
}

pcb_t* pcb[NPROCESS];
pcb_t* current_process;

void init_process() {
    stdin.read = &read_tty;
    stdin.write = 0;
    stdin.open = &open_tty;
    stdin.close = &close_tty;

    stdout.read = 0;
    stdout.write = &write_tty;
    stdout.open = &open_tty;
    stdout.close = &close_tty;

    _SET_FOPS(rtc);
    _SET_FOPS(dir);
    _SET_FOPS(file);

    uint32_t i;
    for (i = 0; i < NPROCESS; ++i) {
        pcb[i] = (pcb_t*)(VMEM_KERNEL_BASE - STACK_SIZE * (i + 1));
        pcb[i]->state = 0;

        uint32_t j;
        for (j = 0; j < NFD; ++j) {
            pcb[i]->fd_array[j].inode = 0;
            pcb[i]->fd_array[j].file_pos = 0;
            pcb[i]->fd_array[j].flags = 0;
        }
    }

    current_process = 0;
}

int32_t get_available_pid() {
    int32_t pid;
    for (pid = 0; pid < NPROCESS; ++pid) {
        if (!pcb[pid]->state)
            break;
    }

    if (pid == NPROCESS)
        return -1;

    pcb[pid]->state = 1;

    return pid;
}
