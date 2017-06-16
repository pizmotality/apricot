/* filesystem.c - Functions to interact with the filesystem
 */

#include "filesystem.h"
#include "lib.h"

filesystem_t* filesystem;

void init_filesystem(filesystem_t* start_address) {
    filesystem = start_address;
}

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
    if (inode >= N_INODE)
        return -1;

    length += offset;
    if (filesystem->inode[inode].length < length)
        length = filesystem->inode[inode].length;

    if (offset > length)
        return 0;

    uint32_t i;
    uint32_t nbytes;
    uint32_t nbytes_read;
    for (i = offset / 4096; i < length / 4096; ++i) {
        nbytes = 4096 - (i == offset / 4096) * (offset % 4096) - (i == length / 4096) * (4096 - length % 4096);
        memcpy(buf + nbytes_read, (uint8_t*)&filesystem->data_block[filesystem->inode[inode].data_block[i]] + ((offset % 4096) * (i == offset / 4096)), nbytes);
        nbytes_read += nbytes;
    }

    return nbytes_read;
}

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry) {
    if (fname[0] == '\0')
        return -1;

    uint32_t i;
    for (i = 0; i < 63; ++i) {
        if (!strncmp((int8_t*)fname, (int8_t*)filesystem->boot_block.dentry[i].fname, 32)) {
            strncpy((int8_t*)dentry->fname, (int8_t*)fname, 32);
            dentry->ftype = filesystem->boot_block.dentry[i].ftype;
            dentry->inode_number = filesystem->boot_block.dentry[i].inode_number;
            return 0;
        }
    }

    return -1;
}

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry) {
    if (index >= N_INODE || filesystem->boot_block.dentry[index].fname[0] == '\0')
        return -1;

    strncpy((int8_t*)dentry->fname, (int8_t*)filesystem->boot_block.dentry[index].fname, 32);
    dentry->ftype = filesystem->boot_block.dentry[index].ftype;
    dentry->inode_number = filesystem->boot_block.dentry[index].inode_number;

    return 0;
}
