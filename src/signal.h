/* signal.h - Defines and functions to deliver signals
 */

#ifndef _SIGNAL_H
#define _SGINAL_H

#include "types.h"

#define SIGDIVZ     0
#define SIGSEGV     1
#define SIGINT      2
#define SIGALARM    3
#define SIGUSER1    4
#define NSIGNAL     5

#define SIGALARM_FREQ   0x50000

void deliver_signal();

void queue_signal(uint32_t signum);

#endif /* _SIGNAL_H */
