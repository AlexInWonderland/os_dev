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
 
void kernel_early(void)
{
	terminal_initialize();
}
 
void kernel_main(void)
{
   init_gdt();
   init_idt();
   irq_install();
   timer_install();
   asm volatile("sti");
   printf("hello world!!\n");
   while(1);
//   int i=10/0;
//   printf("%d",i);
 //  asm volatile("int $0x0");
 //  asm volatile("int $35");
 //  asm volatile("int $0x2");
 //  asm volatile("int $0x3");
 //  asm volatile("int $0x4");
 //  asm volatile("int $0x5");
 //  asm volatile("int $0x6");
 //  asm volatile("int $0x7");
 //  asm volatile("int $14");
   //asm volatile("int $0x3");
  // asm volatile("int $0x9");
}
