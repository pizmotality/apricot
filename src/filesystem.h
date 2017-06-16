/* filesystem.h - Defines and function definitions for interacting with the filesystem
 */

#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include "types.h"

typedef struct dentry_t {
    uint8_t fname[32];
    uint32_t ftype;
    uint32_t inode_number;
    uint32_t reserved[6];
} __attribute__((packed)) dentry_t;

typedef struct inode_t {
    uint32_t length;
    uint32_t data_block[1023];
} __attribute__((packed)) inode_t;

typedef struct data_block_t {
    uint8_t data[4096];
} data_block_t;

typedef struct boot_block_t {
    uint32_t dentry_number;
    uint32_t inode_number;
    uint32_t data_block_number;
    uint32_t reserved[13];
    dentry_t dentry[63];
} __attribute__((packed)) boot_block_t;

#define N_INODE         0x24
#define N_DATA_BLOCK    0x4E

typedef struct filesystem_t {
    boot_block_t boot_block;
    inode_t inode[N_INODE];
    data_block_t data_block[N_DATA_BLOCK];
} filesystem_t;

void init_filesystem(filesystem_t* start_address);

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);

#endif /* _FILESYSTEM_H */
