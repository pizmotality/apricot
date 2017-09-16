/* filesystem.c - Functions to interact with the filesystem
 */

#include "filesystem.h"

filesystem_t* filesystem;

void init_filesystem(filesystem_t* start_address) {
    filesystem = start_address;
}
