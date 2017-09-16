/* interrupt.S - Assembly linkage for interrupt handlers
 */

.globl divide_error
.globl debug
.globl NMI
.globl breakpoint
.globl overflow
.globl bound_range_exceeded
.globl invalid_opcode
.globl device_not_available
.globl double_fault
.globl invalid_tss
.globl segment_not_present
.globl stack_fault
.globl general_protection
.globl page_fault
.globl floating_point_error
.globl alignment_check
.globl machine_check
.globl simd_floating_point_exception

.globl pit
.globl keyboard
.globl rtc

.globl system_call

.macro HANDLE_INT INT
\INT :
    pushal
    call    handle_\INT
    popal
    iret
.endm

.macro HANDLE_INT_ERRCODE INT
\INT :
    pushal
    call    handle_\INT
    popal
    addl    $4, %esp
    iret
.endm

HANDLE_INT divide_error
HANDLE_INT debug
HANDLE_INT NMI
HANDLE_INT breakpoint
HANDLE_INT overflow
HANDLE_INT bound_range_exceeded
HANDLE_INT invalid_opcode
HANDLE_INT device_not_available
HANDLE_INT_ERRCODE double_fault
HANDLE_INT_ERRCODE invalid_tss
HANDLE_INT_ERRCODE segment_not_present
HANDLE_INT_ERRCODE stack_fault
HANDLE_INT_ERRCODE general_protection
HANDLE_INT_ERRCODE page_fault
HANDLE_INT floating_point_error
HANDLE_INT alignment_check
HANDLE_INT machine_check
HANDLE_INT simd_floating_point_exception

HANDLE_INT pit
HANDLE_INT keyboard
HANDLE_INT rtc

#define NUMBER_SYSTEM_CALL  6

.align 4
system_call_vector:
.long   halt
.long   execute
.long   read
.long   write
.long   open
.long   close

system_call:
    decl    %eax
    cmpl    $(NUMBER_SYSTEM_CALL), %eax
    jae     invalid_system_call

    pushl   %eax
    pushl   %ebp
    pushl   %edi
    pushl   %esi
    pushl   %edx
    pushl   %ecx
    pushl   %ebx
    call    *system_call_vector(, %eax, 4)
    popl    %ebx
    popl    %ecx
    popl    %ebx
    popl    %esi
    popl    %edi
    popl    %ebp
    addl    $4, %esp
    iret

invalid_system_call:
    movl    $-1, %eax
    iret
