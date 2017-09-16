/* system_call.h - Defines and function definitions for system calls
 */

#ifndef _SYSTEM_CALL_H
#define _SYSTEM_CALL_H

#include "types.h"

typedef struct pcb_t pcb_t;

typedef struct pcb_t {
    uint32_t state;
    pcb_t* parent;
    pcb_t* child;
    uint32_t id;
    uint32_t esp;
    uint32_t ebp;
    uint32_t return_address;
    uint8_t args[128];
    /* file structure */
    /* signal info */
    /* memory info */
} pcb_t;

int32_t halt(uint8_t status);
int32_t execute(const uint8_t* command);
int32_t read(int32_t fd, void* buf, int32_t nbytes);
int32_t write(int32_t fd, const void* buf, int32_t nbytes);
int32_t open(const uint8_t* fname);
int32_t close(int32_t fd);

#endif /* _SYSTEM_CALL_H */
