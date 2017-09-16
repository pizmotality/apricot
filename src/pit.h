/* pit.h - Defines and functions to initialize PIT
 */

#ifndef _PIT_H
#define _PIT_H

#define CHANNEL0                0x40
#define MODE_COMMAND_REGISTER   0x43

#define PIT_MODE    0x36
#define PIT_FREQ    1193180

#define PIT_IRQ

void init_pit();

void handle_pit();

#endif /* _PIT_H */
