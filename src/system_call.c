/* system_call.c - System calls
 */

#include "system_call.h"
#include "signal.h"
#include "process.h"
#include "filesystem.h"
#include "memory.h"
#include "page.h"
#include "x86_desc.h"
#include "lib.h"

int32_t halt(uint8_t status) {
    current_process->state = 0;

    uint32_t i;
    for (i = 0; i < NFD; ++i) {
        if (current_process->fd_array[i].flags & FDOPEN)
            close(i);
    }

    pcb_t* process = current_process;

    if (current_process->parent) {
        current_process = current_process->parent;

        disable_paging();
        map_memory_block(USER_VMEM, USER_MEM + current_process->pid * _4MB_BLOCK, USER);
        enable_paging();

        tss.esp0 = KERNEL_VMEM_BASE - current_process->pid * STACK_SIZE;
    }

    asm volatile("                          \n\
                 movzbl %0, %%eax           \n\
                 movl   %1, %%esp           \n\
                 movl   %2, %%ebp           \n\
                 movl   %3, 4(%%ebp)        \n\
                 jmp    return_from_halt    \n\
                 "
                 :
                 : "r"(status),
                   "r"(process->esp),
                   "r"(process->ebp),
                   "r"(process->return_address)
                 :
                 );

    return 0;
}

int32_t execute(const uint8_t* command) {
    uint32_t i;
    for (i = 0; i < 63; ++i) {
        if (command[i] == ' ' || command[i] == '\0' || command[i] == '\n')
            break;
    }

    uint8_t binary[64];
    /* truncate binary name to 63 bytes */
    strncpy((int8_t*)binary, (int8_t*)command, i);
    binary[i] = '\0';

    dentry_t executable;
    if (read_dentry_by_name(binary, &executable))
        return -1;

    /* check if file is executable */
    uint8_t header[4];
    if (read_data(executable.inode_index, 0, header, 4) != 4)
        return -1;
    if (*((uint32_t*)header) != ELF_HEADER)
        return -1;

    int32_t pid = get_available_pid();
    if (pid < 0)
        return -1;

    pcb_t* parent_process = current_process;

    current_process = pcb[pid];
    current_process->pid = pid;
    current_process->parent = parent_process;
    current_process->state = 1;

    current_process->args[0] = '\0';
    if (command[i++] == ' ')
        strncpy((int8_t*)current_process->args, (int8_t*)command + i, 128 - i);

    asm volatile("                      \n\
                 leal   4(%%ebp), %0    \n\
                 movl   %%ebp, %1       \n\
                 movl   4(%%ebp), %2    \n\
                 "
                 : "=r"(current_process->esp),
                   "=r"(current_process->ebp),
                   "=r"(current_process->return_address)
                 :
                 );

    current_process->fd_array[0].fops_array = &stdin;
    current_process->fd_array[0].flags = FDOPEN | FDREAD;
    current_process->fd_array[1].fops_array = &stdout;
    current_process->fd_array[1].flags = FDOPEN | FDWRITE;

    current_process->sigqueue = 0;
    current_process->sigmask = SIGMASK_DEF;
    for (i = 0; i < NSIGNAL; ++i)
        current_process->sighandlers[i] = 0;

    cli();

    disable_paging();
    map_memory_block(USER_VMEM, USER_MEM + pid * _4MB_BLOCK, USER);
    enable_paging();

    sti();

    read_data(executable.inode_index, 0, (uint8_t*)0x8048000, 0x400000);
    uint32_t* entry_point = (uint32_t*)(*(uint32_t*)0x8048018);

    tss.ss0 = KERNEL_DS;
    tss.esp0 = KERNEL_VMEM_BASE - pid * STACK_SIZE;

    asm volatile("                      \n\
                 pushl  $0x002B         \n\
                 pushl  $0x8400000      \n\
                 pushfl                 \n\
                 popl   %%edx           \n\
                 orl    $0x4200, %%edx  \n\
                 pushl  %%edx           \n\
                 pushl  $0x0023         \n\
                 pushl  %0              \n\
                 movw   $0x002B, %%dx   \n\
                 movw   %%dx, %%ds      \n\
                 iret                   \n\
                 return_from_halt:      \n\
                 "
                 :
                 : "r"(entry_point)
                 : "edx");

    return 0;
}

int32_t read(int32_t fd, void* buf, int32_t nbytes) {
    if (!(fd & FDINVALID) && (current_process->fd_array[fd].flags & FDREAD))
        return current_process->fd_array[fd].fops_array->read(fd, buf, nbytes);

    return -1;
}

int32_t write(int32_t fd, const void* buf, int32_t nbytes) {
    if (!(fd & FDINVALID) && (current_process->fd_array[fd].flags & FDWRITE))
        return current_process->fd_array[fd].fops_array->write(buf, nbytes);

    return -1;
}

int32_t open(const uint8_t* fname) {
    dentry_t file;
    if (read_dentry_by_name(fname, &file))
        return -1;

    uint32_t fd;
    for (fd = 2; fd < NFD; ++fd) {
        if (current_process->fd_array[fd].flags == FDCLOSE)
            break;
    }

    if (fd == NFD)
        return -1;

    switch (file.ftype) {
        case 0:
            current_process->fd_array[fd].fops_array = &fops_rtc;
            current_process->fd_array[fd].flags = FDREAD | FDWRITE;
            break;
        case 1:
            current_process->fd_array[fd].fops_array = &fops_dir;
            current_process->fd_array[fd].flags = FDREAD;
            break;
        case 2:
            current_process->fd_array[fd].fops_array = &fops_file;
            current_process->fd_array[fd].flags = FDREAD;
            break;
        default:
            return -1;
    }

    current_process->fd_array[fd].inode = index_to_inode(file.inode_index);
    current_process->fd_array[fd].file_pos = 0;
    current_process->fd_array[fd].flags |= FDOPEN;

    current_process->fd_array[fd].fops_array->open();

    return fd;
}

int32_t close(int32_t fd) {
    if (!(fd & FDINVALID) && (current_process->fd_array[fd].flags & FDOPEN)) {
        current_process->fd_array[fd].fops_array->close();
        current_process->fd_array[fd].flags = FDCLOSE;

        return 0;
    }

    return -1;
}

int32_t getargs(uint8_t* buf, int32_t nbytes) {
    uint32_t length = strlen((int8_t*)current_process->args);
    if (length + 1 > nbytes)
        return -1;

    strncpy((int8_t*)buf, (int8_t*)current_process->args, nbytes);
    buf[length] = '\0';

    return 0;
}

int32_t vidmap(uint8_t** screen_start) {
    if ((uint32_t)screen_start < USER_VMEM || (uint32_t)screen_start & 0xF7C00000)
        return -1;

    map_memory_page(VIDEO_VMEM_USER, VIDEO_MEM, USER, page_table_user);
    *screen_start = (uint8_t*)VIDEO_VMEM_USER;

    return 0;
}

int32_t set_handler(int32_t signum, void* handler_address) {
    if ((uint32_t)signum < NSIGNAL) {
        current_process->sighandlers[signum] = handler_address;
        current_process->sigmask &= ~(0x1 << signum);

        return 0;
    }

    return -1;
}

int32_t sigreturn(void) {
    int32_t return_value;

    asm volatile("                          \n\
                 leal   0x8(%%ebp), %%edx   \n\
                 movl   0x34(%%ebp), %%ecx  \n\
                 leal   4(%%ecx), %%ecx     \n\
                 pushl  $0x34               \n\
                 pushl  %%ecx               \n\
                 pushl  %%edx               \n\
                 call   memcpy              \n\
                 movl   0x20(%%ebp), %0     \n\
                 "
                 : "=r" (return_value)
                 :
                 :
                 );

    return return_value;
}
