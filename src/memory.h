/* memory.h - Defines and functions to initialize memory
 */

#ifndef _MEMORY_H
#define _MEMORY_H

#include "types.h"

#define BLOCK_4KB   0x1000
#define BLOCK_4MB   0x400000

#define VIDEO_MEM_PHYS      0xB8000
#define KERNEL_MEM_PHYS     0x400000
#define USER_MEM_PHYS       0x800000

#define VIDEO_MEM_VIRT      0xB8000
#define KERNEL_MEM_VIRT     0x400000
#define USER_MEM_VIRT       0x8000000

#define VIDEO_MEM_INDEX     (VIDEO_MEM_VIRT >> 12)
#define KERNEL_MEM_INDEX    (KERNEL_MEM_VIRT >> 12)
#define USER_MEM_INDEX      (USER_MEM_VIRT >> 12)

#define KERNEL_MEM_BASE     (KERNEL_MEM_VIRT + BLOCK_4MB)

void setup_kernel_page();
void setup_kernel_video_page();

void setup_user_page(int32_t pid);
void setup_user_video_page();

#define STACK_SIZE      0x2000

#endif /* _MEMORY_H */
