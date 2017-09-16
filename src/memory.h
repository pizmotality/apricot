/* memory.h - Defines and functions to initialize memory
 */

#ifndef _MEMORY_H
#define _MEMORY_H

#include "types.h"

#define VIDEO_MEM_ADDRESS   0xB8000
#define KERNEL_MEM_ADDRESS  0x400000
#define USER_MEM_ADDRESS    0x800000

#define VIDEO_MEM_INDEX     (VIDEO_MEM_ADDRESS >> 12)

#define KERNEL_BASE_ADDRESS 0x800000

#define STACK_SIZE      0x2000
#define MEM_BLOCK_4MB   0x400000

void setup_kernel_page();
void setup_kernel_video_page();

void setup_user_page(int32_t pid);
void setup_user_video_page();

#endif /* _MEMORY_H */
