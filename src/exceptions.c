/* exceptions.c - Exception handlers
 */

#include "exceptions.h"
#include "signal.h"
#include "lib.h"

void handle_divide_error() {
    kprintf("Divide error\n");
    queue_signal(SIGDIVZ);
}

void handle_debug() {
    kprintf("Debug\n");
    queue_signal(SIGSEGV);
}

void handle_NMI() {
    kprintf("NMI\n");
    queue_signal(SIGSEGV);
}

void handle_breakpoint() {
    kprintf("Breakpoint\n");
    queue_signal(SIGSEGV);
}

void handle_overflow() {
    kprintf("Overflow\n");
    queue_signal(SIGSEGV);
}

void handle_bound_range_exceeded() {
    kprintf("Bound range exceeded\n");
    queue_signal(SIGSEGV);
}

void handle_invalid_opcode() {
    kprintf("Invalid opcode\n");
    queue_signal(SIGSEGV);
}

void handle_device_not_available() {
    kprintf("Device not available\n");
    queue_signal(SIGSEGV);
}

void handle_double_fault() {
    kprintf("Double fault\n");
    queue_signal(SIGSEGV);
}

void handle_invalid_tss() {
    kprintf("Invalid TSS\n");
    queue_signal(SIGSEGV);
}

void handle_segment_not_present() {
    kprintf("Segment not present\n");
    queue_signal(SIGSEGV);
}

void handle_stack_fault() {
    kprintf("Stack fault\n");
    queue_signal(SIGSEGV);
}

void handle_general_protection() {
    kprintf("General protection\n");
    queue_signal(SIGSEGV);
}

void handle_page_fault() {
    kprintf("Page fault\n");
    queue_signal(SIGSEGV);
}

void handle_floating_point_error() {
    kprintf("Floating point error\n");
    queue_signal(SIGSEGV);
}

void handle_alignment_check() {
    kprintf("Alignment check\n");
    queue_signal(SIGSEGV);
}

void handle_machine_check() {
    kprintf("Machine check\n");
    queue_signal(SIGSEGV);
}

void handle_simd_floating_point_exception() {
    kprintf("SIMD floating point exception\n");
    queue_signal(SIGSEGV);
}
