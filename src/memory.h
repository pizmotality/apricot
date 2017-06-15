/* memory.h - Defines and functions to initialize memory
 */

#ifndef _MEMORY_H
#define _MEMORY_H

#include "types.h"

#define KERNEL_MEM  0x400
#define VIDEO_MEM   0xB8
#define USER_MEM    0x800

extern void setup_kernel_page();
extern void setup_kernel_video_page();

extern void setup_user_page();
extern void setup_user_video_page();

#endif /* _MEMORY_H */
