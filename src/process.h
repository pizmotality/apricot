/* process.h - Defines and function definitions for controlling processes
 */

#ifndef _PROCESS_H
#define _PROCESS_H

#include "types.h"

#define NPROCESS    4
#define NFD         8

typedef struct fops_t {
    int32_t (*read)(int32_t, int8_t*, int32_t);
    int32_t (*write)(const int8_t*, int32_t);
    int32_t (*open)();
    int32_t (*close)();
} fops_t;

struct inode_t;

typedef struct fd_t {
    fops_t* fops_array;
    struct inode_t* inode;
    uint32_t file_pos;
    uint32_t flags;
} fd_t;

typedef struct pcb_t pcb_t;

typedef struct pcb_t {
    uint32_t state;
    uint32_t esp;
    uint32_t ebp;
    uint32_t return_address;
    pcb_t* parent;
    uint8_t args[128];
    fd_t fd_array[8];
    /* signal info */
    /* memory info */
} pcb_t;

void init_process();

int32_t get_available_pid();

pcb_t* get_current_process();
pcb_t* get_process(int32_t pid);

void set_current_process(int32_t pid);

#endif /* _PROCESS_H */
