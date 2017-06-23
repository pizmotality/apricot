/* system_call.c - System calls
 */

#include "system_call.h"
#include "filesystem.h"
#include "process.h"
#include "memory.h"
#include "page.h"
#include "x86_desc.h"
#include "lib.h"

int32_t halt(uint8_t status) {
    pcb_t* current_process = get_current_process();

    uint32_t return_value = (uint32_t)status;
    uint32_t esp = current_process->esp;
    uint32_t ebp = current_process->ebp;
    uint32_t return_address = current_process->return_address;

    current_process->state = 0;

    uint32_t i;
    for (i = 0; i < NFD; ++i) {
        if (current_process->fd_array[i].flags)
            close(i);
        current_process->fd_array[i].flags = 0;
    }

    if (current_process->parent) {
        current_process = current_process->parent;

        disable_paging();
        map_memory_block(USER_VMEM, USER_MEM + current_process->pid * _4MB_BLOCK, USER);
        enable_paging();

        tss.esp0 = KERNEL_VMEM_BASE - current_process->pid * STACK_SIZE;
    }

    asm volatile("                      \n\
                 xorl   %%eax, %%eax    \n\
                 movl   %0, %%eax       \n\
                 movl   %1, %%esp       \n\
                 movl   %2, %%ebp       \n\
                 movl   %3, 4(%%ebp)    \n\
                 leave                  \n\
                 ret                    \n\
                 "
                 :
                 : "r"(return_value), "r"(esp), "r"(ebp), "r"(return_address)
                 :
                 );

    return 0;
}

int32_t execute(const uint8_t* command) {
    /* parse arguments */

    dentry_t executable;
    if (read_dentry_by_name(command, &executable)) {
        printf("executable not found\n");
        return -1;
    }

    /* check if file is executable */
    uint8_t header[4];
    if (read_data(executable.inode_index, 0, header, 4) != 4)
        return -1;

    int32_t pid = get_available_pid();
    if (pid < 0)
        return -1;

    cli();

    pcb_t* process = get_process(pid);
    pcb_t* current_process = get_current_process();

    set_current_process(pid);
    process->parent = current_process;
    process->state = 1;

    /* store arguments */

    uint32_t esp;
    uint32_t ebp;
    uint32_t return_address;
    asm volatile("                      \n\
                 leal   4(%%ebp), %0    \n\
                 movl   %%ebp, %1       \n\
                 movl   4(%%ebp), %2    \n\
                 "
                 : "=c"(esp), "=d"(ebp), "=D"(return_address)
                 :
                 );

    process->esp = esp;
    process->ebp = ebp;
    process->return_address = return_address;

    process->fd_array[0].fops_array = &stdin;
    process->fd_array[0].flags = FDOPEN | FDREAD;
    process->fd_array[1].fops_array = &stdout;
    process->fd_array[1].flags = FDOPEN | FDWRITE;

    sti();

    disable_paging();
    map_memory_block(USER_VMEM, USER_MEM + pid * _4MB_BLOCK, USER);
    enable_paging();

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
                 "
                 :
                 : "r"(entry_point)
                 : "edx");

    return 0;
}

int32_t read(int32_t fd, void* buf, int32_t nbytes) {
    pcb_t* current_process = get_current_process();
    if (fd < NFD && current_process->fd_array[fd].flags & (FDOPEN | FDREAD))
        return current_process->fd_array[fd].fops_array->read(fd, buf, nbytes);

    return -1;
}

int32_t write(int32_t fd, const void* buf, int32_t nbytes) {
    pcb_t* current_process = get_current_process();
    if (fd < NFD && current_process->fd_array[fd].flags & (FDOPEN | FDWRITE))
        return current_process->fd_array[fd].fops_array->write(buf, nbytes);

    return -1;
}

int32_t open(const uint8_t* fname) {
    dentry_t file;
    if (read_dentry_by_name(fname, &file)) {
        printf("file no found\n");
        return -1;
    }

    pcb_t* current_process = get_current_process();

    uint32_t fd;
    for (fd = 2; fd < NFD; ++fd) {
        if (!(current_process->fd_array[fd].flags & FDOPEN))
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
    pcb_t* current_process = get_current_process();
    if (fd < NFD && current_process->fd_array[fd].flags & FDOPEN) {
        current_process->fd_array[fd].fops_array->close();
        current_process->fd_array[fd].flags = FDCLOSE;

        return 0;
    }

    return -1;
}

int32_t getargs(uint8_t* buf, int32_t nbytes) {
    return -1;
}

int32_t vidmap(uint8_t** screen_start) {
    return -1;
}

int32_t set_handler(int32_t signum, void* handler_address) {
    return -1;
}

int32_t sigreturn(void) {
    return -1;
}
