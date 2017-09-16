/* signal.c - Signal delivery and execution
 */

#include "signal.h"
#include "system_call.h"
#include "process.h"
#include "lib.h"

extern void* execute_sigreturn;

void deliver_signal() {
    uint32_t signum = current_process->signum;
    if (!signum)
        return;

    uint32_t* sighandler = current_process->sighandlers[signum];
    if (!sighandler) {
        if (signum < SIGALARM) { halt(-1); }
        else { return; }
    }

    current_process->sigmask = 1;
    current_process->signum = 0;

    uint32_t esp;
    asm volatile("                      \n\
                 leal   8(%%ebp), %0    \n\
                 "
                 : "=r" (esp)
                 );

    uint32_t esp_user;
    asm volatile("                      \n\
                 movl   44(%1), %0      \n\
                 "
                 : "=r" (esp_user)
                 : "r" (esp)
                 );

    memcpy((uint8_t*)esp_user - 0x8, (uint8_t*)&execute_sigreturn, 0x8);
    memcpy((uint8_t*)esp_user - 0x3c, (uint8_t*)esp, 0x34);

    esp_user = esp_user - 0x44;

    asm volatile("                      \n\
                 movl   %1, 4(%0)       \n\
                 leal   60(%0), %%edi   \n\
                 movl   %%edi, (%0)     \n\
                 "
                 :
                 : "r" (esp_user), "r" (signum)
                 : "edi"
                 );

    asm volatile("                      \n\
                 movl   %1, 32(%0)      \n\
                 movl   %2, 44(%0)      \n\
                 "
                 :
                 : "r" (esp), "r" (sighandler), "r" (esp_user)
                 );

    current_process->sigmask = 0;
}

void queue_signal(uint32_t signum) {
    if (current_process->sigmask)
        return;

    current_process->signum = signum;
}
