/* system_call.c - System calls
 */

#include "system_call.h"
#include "filesystem.h"
#include "process.h"
#include "terminal.h"
#include "memory.h"
#include "page.h"
#include "x86_desc.h"
#include "lib.h"

int32_t halt(uint8_t status) {
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

    pcb_t* new_process = get_process(pid);
    pcb_t* current_process = get_current_process();

    new_process->parent = current_process;
    new_process->pid = pid;
    new_process->state = 1;
    set_current_process(pid);

    /* store arguments */

    terminal_t* current_terminal = get_current_terminal();
    current_terminal->n_process++;

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

    new_process->esp = esp;
    new_process->ebp = ebp;
    new_process->return_address = return_address;

    /* setup file descriptors */

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
