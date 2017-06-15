/* page.h - Structures and functions to enable /disable paging
 */

#ifndef _PAGE_H
#define _PAGE_H

typedef union PDE_t {
    struct {
        uint32_t present : 1;
        uint32_t read_write : 1;
        uint32_t user_supervisor : 1;
        uint32_t page_write_through : 1;
        uint32_t page_cache_disable : 1;
        uint32_t accessed : 1;
        uint32_t zero : 1;
        uint32_t page_size : 1;
        uint32_t global : 1;
        uint32_t available : 3;
        uint32_t page_table_base_address : 20;
    } __attribute__((packed));
} PDE_t;

typedef union PTE_t {
    struct {
        uint32_t present : 1;
        uint32_t read_write : 1;
        uint32_t user_supervisor : 1;
        uint32_t page_write_through : 1;
        uint32_t page_cache_disable : 1;
        uint32_t accessed : 1;
        uint32_t dirty : 1;
        uint32_t page_attribute_table : 1;
        uint32_t global : 1;
        uint32_t available : 3;
        uint32_t page_base_address : 20;
    } __attribute__((packed));
} PTE_t;

extern PDE_t page_directory[1024];

extern PTE_t page_table_video[1024];
extern PTE_t page_table_video_user[1024];

extern void init_paging(uint32_t page_directory_address);
extern void enable_paging();
extern void disable_paging();

#endif /* _PAGE_H */
