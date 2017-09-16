/* exceptions.c - Exception handlers
 */

#include "exceptions.h"
#include "signal.h"
#include "lib.h"

void handle_divide_error() {
    printf("Divide error\n");
    queue_signal(SIGDIVZ);
}

void handle_debug() {
    printf("Debug\n");
    queue_signal(SIGSEGV);
}

void handle_NMI() {
    printf("NMI\n");
    queue_signal(SIGSEGV);
}

void handle_breakpoint() {
    printf("Breakpoint\n");
    queue_signal(SIGSEGV);
}

void handle_overflow() {
    printf("Overflow\n");
    queue_signal(SIGSEGV);
}

void handle_bound_range_exceeded() {
    printf("Bound range exceeded\n");
    queue_signal(SIGSEGV);
}

void handle_invalid_opcode() {
    printf("Invalid opcode\n");
    queue_signal(SIGSEGV);
}

void handle_device_not_available() {
    printf("Device not available\n");
    queue_signal(SIGSEGV);
}

void handle_double_fault() {
    printf("Double fault\n");
    queue_signal(SIGSEGV);
}

void handle_invalid_tss() {
    printf("Invalid TSS\n");
    queue_signal(SIGSEGV);
}

void handle_segment_not_present() {
    printf("Segment not present\n");
    queue_signal(SIGSEGV);
}

void handle_stack_fault() {
    printf("Stack fault\n");
    queue_signal(SIGSEGV);
}

void handle_general_protection() {
    printf("General protection\n");
    queue_signal(SIGSEGV);
}

void handle_page_fault() {
    printf("Page fault\n");
    queue_signal(SIGSEGV);
}

void handle_floating_point_error() {
    printf("Floating point error\n");
    queue_signal(SIGSEGV);
}

void handle_alignment_check() {
    printf("Alignment check\n");
    queue_signal(SIGSEGV);
}

void handle_machine_check() {
    printf("Machine check\n");
    queue_signal(SIGSEGV);
}

void handle_simd_floating_point_exception() {
    printf("SIMD floating point exception\n");
    queue_signal(SIGSEGV);
}
