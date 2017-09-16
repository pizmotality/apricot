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
    pushl   $0
    pushl   %eax
    pushl   %ebp
    pushl   %edi
    pushl   %esi
    pushl   %edx
    pushl   %ecx
    pushl   %ebx
    call    handle_\INT
    jmp     check_signal
.endm

.macro HANDLE_INT_ERRCODE INT
\INT :
    pushl   %eax
    pushl   %ebp
    pushl   %edi
    pushl   %esi
    pushl   %edx
    pushl   %ecx
    pushl   %ebx
    call    handle_\INT
    jmp     check_signal
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

#define NUMBER_SYSTEM_CALL  10

.align 4
system_call_vector:
.long   halt
.long   execute
.long   read
.long   write
.long   open
.long   close
.long   getargs
.long   vidmap
.long   set_handler
.long   sigreturn

system_call:
    decl    %eax

    pushl   $0
    pushl   %eax
    pushl   %ebp
    pushl   %edi
    pushl   %esi
    pushl   %edx
    pushl   %ecx
    pushl   %ebx

    cmpl    $(NUMBER_SYSTEM_CALL), %eax
    jae     invalid_system_call

    call    *system_call_vector(, %eax, 4)
    movl    %eax, 24(%esp)
    jmp     check_signal

invalid_system_call:
    movl    $-1, 24(%esp)

.align 4
check_signal:
    cmpl    $0x8000000, 44(%esp)
    jb      return_from_interrupt

    call    deliver_signal

return_from_interrupt:
    popl    %ebx
    popl    %ecx
    popl    %edx
    popl    %esi
    popl    %edi
    popl    %ebp
    popl    %eax
    addl    $4, %esp
    iret

.globl execute_sigreturn

.align 8
execute_sigreturn:
    movl    $0xa, %eax
    int     $0x80

.align 8
