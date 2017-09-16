/* memory.h - Defines and functions to initialize memory
 */

#ifndef _MEMORY_H
#define _MEMORY_H

#define VIDEO_MEM_ADDRESS   0xB8000
#define KERNEL_MEM_ADDRESS  0x400000
#define USER_MEM_ADDRESS    0x800000

#define KERNEL_BASE_ADDRESS 0x800000

#define STACK_SIZE   0x2000

#define VIDEO_MEM_INDEX     (VIDEO_MEM_ADDRESS >> 12)

extern void setup_kernel_page();
extern void setup_kernel_video_page();

extern void setup_user_page();
extern void setup_user_video_page();

#endif /* _MEMORY_H */
