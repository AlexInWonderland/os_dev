#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
 
#include <kernel/tty.h>
#include <kernel/vga.h>
#include <kernel/common.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/timer.h>
#include <kernel/key.h>
 
void kernel_early(void)
{
	terminal_initialize();
}

static void kernel_init()
{
   init_gdt();
   init_idt();
   irq_install();
   timer_install();
   keyboard_install();
   asm volatile("sti");    //this is a must-do
}
 
void kernel_main(void)
{
   kernel_init();
   task_tty();
//   printf("%i\n", 251);
//   printf("%i\n", 252);
//   printf("%i\n",253);
//   printf("%i\n", 254);
//   printf("%i\n", 255);
   while(1);
}
