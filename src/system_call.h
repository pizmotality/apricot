/* system_call.h - Defines and function definitions for system calls
 */

#ifndef _SYSTEM_CALL_H
#define _SYSTEM_CALL_H

#include "types.h"

#define ELF_HEADER  0x464C457F

int32_t halt(uint8_t status);
int32_t execute(const uint8_t* command);
int32_t read(int32_t fd, void* buf, int32_t nbytes);
int32_t write(int32_t fd, const void* buf, int32_t nbytes);
int32_t open(const uint8_t* fname);
int32_t close(int32_t fd);
int32_t getargs(uint8_t* buf, int32_t nbytes);
int32_t vidmap(uint8_t** screen_start);
int32_t set_handler(int32_t signum, void* handler_address);
int32_t sigreturn(void);

#endif /* _SYSTEM_CALL_H */
