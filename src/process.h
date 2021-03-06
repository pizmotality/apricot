/* process.h - Defines and function definitions for controlling processes
 */

#ifndef _PROCESS_H
#define _PROCESS_H

#include "signal.h"
#include "types.h"

typedef struct fops_t {
    int32_t (*read)(int32_t, int8_t*, int32_t);
    int32_t (*write)(const int8_t*, int32_t);
    int32_t (*open)();
    int32_t (*close)();
} fops_t;

extern fops_t stdin;
extern fops_t stdout;
extern fops_t fops_rtc;
extern fops_t fops_dir;
extern fops_t fops_file;

#define NFD         8

#define FDINVALID   0xFFFFFFF8

#define FDCLOSE     0x0
#define FDOPEN      0x1
#define FDREAD      0x2
#define FDWRITE     0x4

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
    int32_t pid;
    uint32_t parent_esp;
    uint32_t parent_ebp;
    uint32_t parent_eip;
    pcb_t* parent;
    uint8_t args[128];
    fd_t fd_array[NFD];
    uint32_t sigmask;
    uint32_t sigqueue;
    uint32_t* sighandlers[NSIGNAL];
    uint32_t tty;
    uint32_t esp;
    uint32_t ebp;
} pcb_t;

#define NPROCESS    6

#define PAVAIL      0x0
#define PALIVE      0x1
#define PACTIVE     0x2

extern pcb_t* pcb[NPROCESS];
extern pcb_t* current_process;

void init_process();

int32_t get_available_pid();

#endif /* _PROCESS_H */
