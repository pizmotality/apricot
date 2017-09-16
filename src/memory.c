/* memory.c - Functions to set up memory
 */

#include "memory.h"
#include "page.h"

void setup_kernel_page() {
    page_directory[1].present = 1;
    page_directory[1].read_write = 1;
    page_directory[1].user_supervisor = 0;
    page_directory[1].page_write_through = 0;
    page_directory[1].page_cache_disable = 0;
    page_directory[1].zero = 0;
    page_directory[1].page_size = 1;
    page_directory[1].global = 1;
    page_directory[KERNEL_MEM_VIRT >> 22].page_table_base_address = KERNEL_MEM_PHYS >> 12;
}

void setup_kernel_video_page() {
    page_directory[0].present = 1;
    page_directory[0].read_write = 1;
    page_directory[0].user_supervisor = 1;
    page_directory[0].page_write_through = 0;
    page_directory[0].page_cache_disable = 0;
    page_directory[0].zero = 0;
    page_directory[0].page_size = 0;
    page_directory[0].global = 0;
    page_directory[0].page_table_base_address = (uint32_t)page_table >> 12;

    page_table[VIDEO_MEM_INDEX].present = 1;
    page_table[VIDEO_MEM_INDEX].read_write = 1;
    page_table[VIDEO_MEM_INDEX].user_supervisor = 1;
    page_table[VIDEO_MEM_INDEX].page_write_through = 0;
    page_table[VIDEO_MEM_INDEX].dirty = 1;
    page_table[VIDEO_MEM_INDEX].page_attribute_table = 0;
    page_table[VIDEO_MEM_INDEX].global = 0;
    page_table[VIDEO_MEM_INDEX].page_base_address = VIDEO_MEM_PHYS >> 12;
}

void setup_user_page(int32_t pid) {
    page_directory[0x20].present = 1;
    page_directory[0x20].read_write = 1;
    page_directory[0x20].user_supervisor = 1;
    page_directory[0x20].page_write_through = 0;
    page_directory[0x20].page_cache_disable = 0;
    page_directory[0x20].zero = 0;
    page_directory[0x20].page_size = 1;
    page_directory[0x20].global = 0;
    page_directory[0x20].page_table_base_address = (USER_MEM_PHYS + BLOCK_4MB * pid) >> 12;
}

void setup_user_video_page() {
    page_directory[0x21].present = 1;
    page_directory[0x21].read_write = 1;
    page_directory[0x21].user_supervisor = 1;
    page_directory[0x21].page_write_through = 0;
    page_directory[0x21].page_cache_disable = 1;
    page_directory[0x21].zero = 0;
    page_directory[0x21].page_size = 0;
    page_directory[0x21].global = 0;
    page_directory[0x21].page_table_base_address = (uint32_t)page_table_user >> 12;

    page_table_user[VIDEO_MEM_INDEX].present = 1;
    page_table_user[VIDEO_MEM_INDEX].read_write = 1;
    page_table_user[VIDEO_MEM_INDEX].user_supervisor = 1;
    page_table_user[VIDEO_MEM_INDEX].page_write_through = 0;
    page_table_user[VIDEO_MEM_INDEX].dirty = 1;
    page_table_user[VIDEO_MEM_INDEX].page_attribute_table = 0;
    page_table_user[VIDEO_MEM_INDEX].global = 0;
    page_table_user[VIDEO_MEM_INDEX].page_base_address = VIDEO_MEM_PHYS >> 12;
}
