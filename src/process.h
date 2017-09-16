/* process.h - Defines and function definitions for controlling processes
 */

#ifndef _PROCESS_H
#define _PROCESS_H

#include "types.h"

#define NPROCESS    4

typedef struct pcb_t pcb_t;

typedef struct pcb_t {
    uint32_t state;
    uint32_t esp;
    uint32_t ebp;
    uint32_t return_address;
    pcb_t* parent;
    uint8_t args[128];
    /* file structure */
    /* signal info */
    /* memory info */
} pcb_t;

void init_process();

int32_t get_available_pid();

pcb_t* get_current_process();
pcb_t* get_process(int32_t pid);

void set_current_process(int32_t pid);

#endif /* _PROCESS_H */
