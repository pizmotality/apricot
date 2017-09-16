/* system_call.c - System calls
 */

#include "system_call.h"

int32_t halt(uint8_t status) {
    return 0;
}

int32_t execute(const uint8_t* command) {
    return 0;
}

int32_t read(int32_t fd, void* buf, int32_t nbytes) {
    return 0;
}

int32_t write(int32_t fd, const void* buf, int32_t nbytes) {
    return 0;
}

int32_t open(const uint8_t* fname) {
    return 0;
}

int32_t close(int32_t fd) {
    return 0;
}
