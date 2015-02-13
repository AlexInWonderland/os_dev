#ifndef _KERNEL_TIMER_H
#define _KERNEL_TIMER_H
/* TIMER.C */
#include <kernel/isr.h>
void timer_wait(int ticks);
void timer_install();
void timer_handler(struct regs *r);
void timer_install();
#endif
