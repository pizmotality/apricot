/* schedule.h - Function definitions for scheduling processes
 */

#ifndef _SCHEDULE_H
#define _SCHEDULE_H

#include "types.h"

int32_t sched_next();
void raise(int32_t pid);

#endif /* _SCHEDULE_H */
