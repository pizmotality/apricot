/* memory.h - Defines and functions to initialize memory
 */

#ifndef _MEMORY_H
#define _MEMORY_H

#include "types.h"

struct PTE_t;

#define USER        1
#define SUPERVISOR  0

#define PMEM_VIDEO      0xB8000
#define VMEM_VIDEO      0xB9000

#define PMEM_KERNEL     0x400000
#define VMEM_KERNEL     0x400000

#define PMEM_USER       0x800000
#define VMEM_USER       0x8000000

void map_memory_block(uint32_t virtual, uint32_t physical, uint32_t user_supervisor);
void map_memory_page(uint32_t virtual, uint32_t physical, uint32_t user_supervisor, struct PTE_t* page_table);

#define MEM_PAGE    0x1000
#define MEM_BLOCK   0x400000
#define STACK_SIZE  0x2000

#define VMEM_KERNEL_BASE    (VMEM_KERNEL + MEM_BLOCK)

#define VMEM_VIDEO_USER     (VMEM_USER + MEM_BLOCK)
#define VMEM_VIDEO_PERSIST  (PMEM_VIDEO)
#define PMEM_VIDEO_BUFFER   (PMEM_VIDEO + MEM_PAGE)

#endif /* _MEMORY_H */
