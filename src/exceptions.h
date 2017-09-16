/* exceptions.h
 */

#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

void handle_divide_error();
void handle_debug();
void handle_NMI();
void handle_breakpoint();
void handle_overflow();
void handle_bound_range_exceeded();
void handle_invalid_opcode();
void handle_device_not_available();
void handle_double_fault();
void handle_invalid_tss();
void handle_segment_not_present();
void handle_stack_fault();
void handle_general_protection();
void handle_page_fault();
void handle_floating_point_error();
void handle_alignment_check();
void handle_machine_check();
void handle_simd_floating_point_exception();

#endif /* _EXCEPTIONS_H */
