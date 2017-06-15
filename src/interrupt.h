/* interrupt.h - Function definitions for interrupt handlers
 */

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

void divide_error();
void debug();
void NMI();
void breakpoint();
void overflow();
void bound_range_exceeded();
void invalid_opcode();
void device_not_available();
void double_fault();
void invalid_tss();
void segment_not_present();
void stack_fault();
void general_protection();
void page_fault();
void floating_point_error();
void alignment_check();
void machine_check();
void simd_floating_point_exception();

void pit();
void keyboard();
void rtc();

#endif /* _INTERRUPT_H */
