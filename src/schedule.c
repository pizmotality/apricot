/* schedule.c - Scheduling
 */

#include "schedule.h"
#include "tty.h"
#include "process.h"
#include "memory.h"
#include "page.h"
#include "lib.h"
#include "x86_desc.h"

int32_t sched_next() {
    if (!current_process)
        return -1;

    uint32_t i;
    for (i = 1; i < NPROCESS; ++i) {
        int32_t pid = (current_process->pid + i) % NPROCESS;
        if (pcb[pid]->state &= PACTIVE)
            return pid;
    }

    return -1;
}

void raise(int32_t pid) {
    asm volatile("                      \n\
                 movl   %%esp, %0       \n\
                 movl   %%ebp, %1       \n\
                 "
                 : "=r"(current_process->esp),
                   "=r"(current_process->ebp)
                 :
                 );

    cli();

    disable_paging();
    map_memory_block(VMEM_USER, PMEM_USER + pid * MEM_BLOCK, USER);
    enable_paging();

    current_process = pcb[pid];

    uint32_t video_memory = 0;
    if (current_tty == current_process->tty)
        video_memory = PMEM_VIDEO;
    else
        video_memory = PMEM_VIDEO_BUFFER + current_process->tty * MEM_PAGE;

    if (video_memory)
        remap_memory_video(video_memory);

    tss.esp0 = VMEM_KERNEL_BASE - STACK_SIZE - pid * STACK_SIZE;

    sti();

    asm volatile("                      \n\
                 movl   %0, %%esp       \n\
                 movl   %1, %%ebp       \n\
                 leave                  \n\
                 ret                    \n\
                 "
                 :
                 : "r"(current_process->esp),
                   "r"(current_process->ebp)
                 );
}
