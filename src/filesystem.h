/* filesystem.h - Defines and function definitions for interacting with the filesystem
 */

#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include "types.h"

typedef struct dentry_t {
    uint8_t fname[32];
    uint32_t ftype;
    uint32_t inode_index;
    uint32_t reserved[6];
} __attribute__((packed)) dentry_t;

#define NDENTRY         0x3F

typedef struct boot_block_t {
    uint32_t ndentry;
    uint32_t ninode;
    uint32_t ndata;
    uint32_t reserved[13];
    dentry_t dentry_block[NDENTRY];
} __attribute__((packed)) boot_block_t;

typedef struct inode_t {
    uint32_t length;
    uint32_t data_index[1023];
} __attribute__((packed)) inode_t;

typedef struct data_block_t {
    uint8_t raw[4096];
} data_block_t;

#define NINODE          0x24
#define NDATABLOCK      0x4E

typedef struct filesystem_t {
    boot_block_t boot_block;
    inode_t inode_block[NINODE];
    data_block_t data[NDATABLOCK];
} filesystem_t;

void init_filesystem(filesystem_t* start_address);

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);

inode_t* index_to_inode(uint32_t index);
uint32_t inode_to_index(inode_t* inode);

int32_t open_dir();
int32_t close_dir();
int32_t read_dir(int32_t fd, int8_t* buf, int32_t nbytes);
int32_t write_dir(const int8_t* buf, int32_t nbytes);

int32_t read_file(int32_t fd, int8_t* buf, int32_t nbytes);
int32_t write_file(const int8_t* buf, int32_t nbytes);
int32_t open_file();
int32_t close_file();

#endif /* _FILESYSTEM_H */
