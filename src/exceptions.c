/* exceptions.c - Exception handlers
 */

#include "exceptions.h"
#include "lib.h"

void handle_divide_error() {
    printf("Divide error\n");
}

void handle_debug() {
    printf("Debug\n");
}

void handle_NMI() {
    printf("NMI\n");
}

void handle_breakpoint() {
    printf("Breakpoint\n");
}

void handle_overflow() {
    printf("Overflow\n");
}

void handle_bound_range_exceeded() {
    printf("Bound range exceeded\n");
}

void handle_invalid_opcode() {
    printf("Invalid opcode\n");
}

void handle_device_not_available() {
    printf("Device not available\n");
}

void handle_double_fault() {
    printf("Double fault\n");
}

void handle_invalid_tss() {
    printf("Invalid TSS\n");
}

void handle_segment_not_present() {
    printf("Segment not present\n");
}

void handle_stack_fault() {
    printf("Stack fault\n");
}

void handle_general_protection() {
    printf("General protection\n");
}

void handle_page_fault() {
    printf("Page fault\n");
}

void handle_floating_point_error() {
    printf("Floating point error\n");
}

void handle_alignment_check() {
    printf("Alignment check\n");
}

void handle_machine_check() {
    printf("Machine check\n");
}

void handle_simd_floating_point_exception() {
    printf("SIMD floating point exception\n");
}
