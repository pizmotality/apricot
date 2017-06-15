/* idt.c - Initialization of the IDT
 */

#include "idt.h"
#include "x86_desc.h"
#include "interrupt.h"
#include "types.h"

void init_idt() {
    idt_desc_t interrupt_gate;
    interrupt_gate.seg_selector = KERNEL_CS;
    interrupt_gate.reserved4 = 0;
    interrupt_gate.reserved3 = 0;
    interrupt_gate.reserved2 = 1;
    interrupt_gate.reserved1 = 1;
    interrupt_gate.size = 1;
    interrupt_gate.reserved0 = 0;
    interrupt_gate.dpl = 0;
    interrupt_gate.present = 1;

    idt_desc_t trap_gate;
    trap_gate.seg_selector = KERNEL_CS;
    trap_gate.reserved4 = 0;
    trap_gate.reserved3 = 1;
    trap_gate.reserved2 = 1;
    trap_gate.reserved1 = 1;
    trap_gate.size = 1;
    trap_gate.reserved0 = 0;
    trap_gate.dpl = 0;
    trap_gate.present = 1;

    idt[0] = interrupt_gate;
    SET_IDT_ENTRY(idt[0], divide_error);
    idt[1] = interrupt_gate;
    SET_IDT_ENTRY(idt[1], debug);
    idt[2] = interrupt_gate;
    SET_IDT_ENTRY(idt[2], NMI);
    idt[3] = interrupt_gate;
    SET_IDT_ENTRY(idt[3], breakpoint);
    idt[4] = interrupt_gate;
    SET_IDT_ENTRY(idt[4], overflow);
    idt[5] = interrupt_gate;
    SET_IDT_ENTRY(idt[5], bound_range_exceeded);
    idt[6] = interrupt_gate;
    SET_IDT_ENTRY(idt[6], invalid_opcode);
    idt[7] = interrupt_gate;
    SET_IDT_ENTRY(idt[7], device_not_available);
    idt[8] = interrupt_gate;
    SET_IDT_ENTRY(idt[8], double_fault);
    // idt[9] is reserved
    idt[10] = interrupt_gate;
    SET_IDT_ENTRY(idt[10], invalid_tss);
    idt[11] = interrupt_gate;
    SET_IDT_ENTRY(idt[11], segment_not_present);
    idt[12] = interrupt_gate;
    SET_IDT_ENTRY(idt[12], stack_fault);
    idt[13] = interrupt_gate;
    SET_IDT_ENTRY(idt[13], general_protection);
    idt[14] = interrupt_gate;
    SET_IDT_ENTRY(idt[14], page_fault);
    // idt[15] is reserved
    idt[16] = interrupt_gate;
    SET_IDT_ENTRY(idt[16], floating_point_error);
    idt[17] = interrupt_gate;
    SET_IDT_ENTRY(idt[17], alignment_check);
    idt[18] = interrupt_gate;
    SET_IDT_ENTRY(idt[18], machine_check);
    idt[19] = interrupt_gate;
    SET_IDT_ENTRY(idt[19], simd_floating_point_exception);

    idt[INT_PIT] = interrupt_gate;
    SET_IDT_ENTRY(idt[INT_PIT], pit);

    idt[INT_KEYBOARD] = interrupt_gate;
    SET_IDT_ENTRY(idt[INT_KEYBOARD], keyboard);

    idt[INT_RTC] = interrupt_gate;
    SET_IDT_ENTRY(idt[INT_RTC], rtc);

    // idt[INT_SYSCALL] = trap_gate;
    // idt[INT_SYSCALL].dpl = 3;
    // SET_IDT_ENTRY(idt[INT_SYSCAL], system_call);
}
