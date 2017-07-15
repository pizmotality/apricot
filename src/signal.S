/* signal.S - Signal delivery
 */

#define ASM

#include "signal.h"

.globl  deliver_signal

.align 4
deliver_signal:
    pushl   %ebp
    movl    %esp, %ebp
    movl    current_process, %esi
    movl    0x11c(%esi), %eax
    xorl    %ecx, %ecx

check_signal:
    shr     $0x1, %eax
    jc      handle_signal
    incl    %ecx
    cmpl    $0x5, %ecx
    je      leave_deliver_signal
    jmp     check_signal

handle_signal:
    movl    $0x1, %eax
    shl     %cl, %eax
    movl    %eax, 0x11c(%esi)
    leal    0x120(%esi), %ebx
    movl    (%ebx, %ecx, 4), %ebx
    testl   %ebx, %ebx
    jne     setup_handler
    movl    $0x1, %edx
    shl     %cl, %edx
    andl    $SIGMASK_INT, %edx
    je      leave_deliver_signal
    pushl   $0xff
    call    halt

setup_handler:
    movl    0x118(%esi), %edi
    movl    $SIGMASK_ALL, 0x118(%esi)
    movl    $0, 0x11c(%esi)
    leal    0x8(%ebp), %eax
    movl    0x2c(%eax), %edx
    subl    $0x3c, %edx
    movl    %ecx, -4(%edx)
    leal    0x34(%edx), %ecx
    movl    %ecx, -8(%edx)
    leal    execute_sigreturn, %ecx
    movl    (%ecx), %edi
    movl    %edi, 0x34(%edx)
    movl    4(%ecx), %edi
    movl    %edi, 0x38(%edx)
    pushl   $0x34
    pushl   %eax
    pushl   %edx
    call    memcpy
    popl    %edx
    popl    %eax
    subl    $0x8, %esp
    movl    %ebx, 0x20(%eax)
    subl    $0x8, %edx
    movl    %edx, 0x2c(%eax)
    movl    %edi, 0x118(%esi)

.align 4
leave_deliver_signal:
    leave
    ret

.globl  queue_signal

.align 4
queue_signal:
    pushl   %ebp
    movl    %esp, %ebp
    pushl   %eax
    pushl   %ecx
    pushl   %edx
    movl    current_process, %ebx
    movl    0x118(%ebx), %eax
    movl    $0x1, %edx
    movl    8(%ebp), %ecx
    shl     %cl, %edx
    andl    %edx, %eax
    jne     leave_queue_signal
    orl     %edx, 0x11c(%ebx)

leave_queue_signal:
    popl    %edx
    popl    %ecx
    popl    %eax
    leave
    ret
