/* memory.h - Defines and functions to initialize memory
 */

#ifndef _MEMORY_H
#define _MEMORY_H

#include "types.h"

struct PTE_t;

#define USER        1
#define SUPERVISOR  0

#define VIDEO_MEM       0xB8000
#define VIDEO_VMEM      0xB8000
#define VIDEO_VMEM_USER 0x8400000

#define KERNEL_MEM      0x400000
#define KERNEL_VMEM     0x400000

#define USER_MEM        0x800000
#define USER_VMEM       0x8000000

void map_memory_block(uint32_t virtual, uint32_t physical, uint32_t user_supervisor);
void map_memory_page(uint32_t virtual, uint32_t physical, uint32_t user_supervisor, struct PTE_t* page_table);

#define _4KB_BLOCK  0x1000
#define _4MB_BLOCK  0x400000
#define STACK_SIZE  0x2000

#define KERNEL_VMEM_BASE    (KERNEL_VMEM + _4MB_BLOCK)

#endif /* _MEMORY_H */
