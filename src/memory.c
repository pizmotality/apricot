/* memory.c - Functions to set up memory
 */

#include "memory.h"
#include "page.h"

void map_memory_block(uint32_t virtual, uint32_t physical, uint32_t user_supervisor) {
    uint32_t index_pd = virtual >> 22;

    page_directory[index_pd].present = 1;
    page_directory[index_pd].read_write = 1;
    page_directory[index_pd].user_supervisor = user_supervisor;
    page_directory[index_pd].page_write_through = 0;
    page_directory[index_pd].page_cache_disable = 0;
    page_directory[index_pd].zero = 0;
    page_directory[index_pd].page_size = 1;
    page_directory[index_pd].global = 0;
    page_directory[index_pd].page_table_base_address = physical >> 12;
}

void map_memory_page(uint32_t virtual, uint32_t physical, uint32_t user_supervisor, PTE_t* page_table) {
    uint32_t index_pd = virtual >> 22;
    uint32_t index_pt = (virtual >> 12) & 0x3FF;

    page_directory[index_pd].present = 1;
    page_directory[index_pd].read_write = 1;
    page_directory[index_pd].user_supervisor = user_supervisor;
    page_directory[index_pd].page_write_through = 0;
    page_directory[index_pd].page_cache_disable = 0;
    page_directory[index_pd].zero = 0;
    page_directory[index_pd].page_size = 0;
    page_directory[index_pd].global = 0;
    page_directory[index_pd].page_table_base_address = (uint32_t)page_table >> 12;

    page_table[index_pt].present = 1;
    page_table[index_pt].read_write = 1;
    page_table[index_pt].user_supervisor = user_supervisor;
    page_table[index_pt].page_write_through = 0;
    page_table[index_pt].dirty = 1;
    page_table[index_pt].page_attribute_table = 0;
    page_table[index_pt].global = 0;
    page_table[index_pt].page_base_address = physical >> 12;
}

void remap_memory_video(uint32_t physical) {
    map_memory_page(VMEM_VIDEO, physical, SUPERVISOR, page_table);
    if (page_table_user[(VMEM_VIDEO_USER >> 12) & 0x3FF].present)
        map_memory_page(VMEM_VIDEO_USER, physical, USER, page_table_user);
}
