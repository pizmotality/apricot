/* memory.c - Functions to set up memory
 */

#include "memory.h"
#include "page.h"

extern void setup_kernel_page() {
    page_directory[1].present = 1;
    page_directory[1].read_write = 1;
    page_directory[1].user_supervisor = 0;
    page_directory[1].page_write_through = 0;
    page_directory[1].page_cache_disable = 0;
    page_directory[1].zero = 0;
    page_directory[1].page_size = 1;
    page_directory[1].global = 1;
    page_directory[1].page_table_base_address = KERNEL_MEM;
}

extern void setup_kernel_video_page() {
    page_directory[0].present = 1;
    page_directory[0].read_write = 1;
    page_directory[0].user_supervisor = 1;
    page_directory[0].page_write_through = 0;
    page_directory[0].page_cache_disable = 0;
    page_directory[0].zero = 0;
    page_directory[0].page_size = 0;
    page_directory[0].global = 0;
    page_directory[0].page_table_base_address = (uint32_t)page_table_video >> 12;

    page_table_video[VIDEO_MEM].present = 1;
    page_table_video[VIDEO_MEM].read_write = 1;
    page_table_video[VIDEO_MEM].user_supervisor = 1;
    page_table_video[VIDEO_MEM].page_write_through = 0;
    page_table_video[VIDEO_MEM].dirty = 1;
    page_table_video[VIDEO_MEM].page_attribute_table = 0;
    page_table_video[VIDEO_MEM].global = 0;
    page_table_video[VIDEO_MEM].page_base_address = VIDEO_MEM;
}

extern void setup_user_page() {
    page_directory[32].present = 1;
    page_directory[32].read_write = 1;
    page_directory[32].user_supervisor = 1;
    page_directory[32].page_write_through = 0;
    page_directory[32].page_cache_disable = 0;
    page_directory[32].zero = 0;
    page_directory[32].page_size = 1;
    page_directory[32].global = 0;
    page_directory[32].page_table_base_address = USER_MEM;
}

extern void setup_user_video_page() {
    page_directory[33].present = 1;
    page_directory[33].read_write = 1;
    page_directory[33].user_supervisor = 1;
    page_directory[33].page_write_through = 0;
    page_directory[33].page_cache_disable = 1;
    page_directory[33].zero = 0;
    page_directory[33].page_size = 0;
    page_directory[33].global = 0;
    page_directory[33].page_table_base_address = (uint32_t)page_table_video_user >> 12;

    page_table_video[VIDEO_MEM].present = 1;
    page_table_video[VIDEO_MEM].read_write = 1;
    page_table_video[VIDEO_MEM].user_supervisor = 1;
    page_table_video[VIDEO_MEM].page_write_through = 0;
    page_table_video[VIDEO_MEM].dirty = 1;
    page_table_video[VIDEO_MEM].page_attribute_table = 0;
    page_table_video[VIDEO_MEM].global = 0;
    page_table_video[VIDEO_MEM].page_base_address = VIDEO_MEM;
}
