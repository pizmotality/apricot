/* filesystem.c - Functions to interact with the filesystem
 */

#include "filesystem.h"
#include "process.h"
#include "lib.h"

filesystem_t* filesystem;

void init_filesystem(filesystem_t* start_address) {
    filesystem = start_address;
}

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
    if (inode >= NINODE)
        return -1;

    length += offset;
    if (filesystem->inode_block[inode].length < length)
        length = filesystem->inode_block[inode].length;

    if (offset > length)
        return 0;

    uint32_t i;
    uint32_t nbytes_read = 0;
    for (i = offset / 4096; i <= length / 4096; ++i) {
        uint32_t nbytes = 4096 - (i == offset / 4096) * (offset % 4096) - (i == length / 4096) * (4096 - length % 4096);
        memcpy(buf + nbytes_read, (uint8_t*)&filesystem->data[filesystem->inode_block[inode].data_index[i]] + ((offset % 4096) * (i == offset / 4096)), nbytes);
        nbytes_read += nbytes;
    }

    return nbytes_read;
}

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry) {
    if (fname[0] == '\0')
        return -1;

    uint32_t i;
    for (i = 0; i < NDENTRY; ++i) {
        if (!strncmp((int8_t*)fname, (int8_t*)filesystem->boot_block.dentry_block[i].fname, 32)) {
            strncpy((int8_t*)dentry->fname, (int8_t*)fname, 32);
            dentry->ftype = filesystem->boot_block.dentry_block[i].ftype;
            dentry->inode_index = filesystem->boot_block.dentry_block[i].inode_index;
            return 0;
        }
    }

    return -1;
}

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry) {
    if (index >= NINODE || filesystem->boot_block.dentry_block[index].fname[0] == '\0')
        return -1;

    strncpy((int8_t*)dentry->fname, (int8_t*)filesystem->boot_block.dentry_block[index].fname, 32);
    dentry->ftype = filesystem->boot_block.dentry_block[index].ftype;
    dentry->inode_index = filesystem->boot_block.dentry_block[index].inode_index;

    return 0;
}

inode_t* index_to_inode(uint32_t index) {
    return filesystem->inode_block + index;
}

uint32_t inode_to_index(inode_t* inode) {
    return inode - filesystem->inode_block;
}

int32_t read_dir(int32_t fd, int8_t* buf, int32_t nbytes) {
    uint32_t dentry_index = current_process->fd_array[fd].file_pos;
    if (dentry_index == filesystem->boot_block.ndentry)
        return 0;

    ++current_process->fd_array[fd].file_pos;
    strncpy(buf, (int8_t*)filesystem->boot_block.dentry_block[dentry_index].fname, nbytes);

    return 1;
}

int32_t write_dir(const int8_t* buf, int32_t nbytes) {
    return -1;
}

int32_t open_dir() {
    return 0;
}

int32_t close_dir() {
    return 0;
}

int32_t read_file(int32_t fd, int8_t* buf, int32_t nbytes) {
    uint32_t index = inode_to_index(current_process->fd_array[fd].inode);
    uint32_t offset = current_process->fd_array[fd].file_pos;

    uint32_t nbytes_read = read_data(index, offset, (uint8_t*)buf, nbytes);
    if (nbytes_read < nbytes) {
        current_process->fd_array[fd].file_pos = 0;
        return 0;
    } else {
        current_process->fd_array[fd].file_pos += nbytes_read;
        return nbytes_read;
    }
}

int32_t write_file(const int8_t* buf, int32_t nbytes) {
    return -1;
}

int32_t open_file() {
    return 0;
}

int32_t close_file() {
    return 0;
}
