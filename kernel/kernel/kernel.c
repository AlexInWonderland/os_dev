#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
 
#include <kernel/tty.h>
#include <kernel/vga.h>
#include <kernel/common.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
 
void kernel_early(void)
{
	terminal_initialize();
}
 
void kernel_main(void)
{
   init_gdt();
  // isrs_install();
   init_idt();
   int a=5;
   int b=451;
//   char* ab="abcd";
    printf("a:%d",b);
    //printf("1");
   asm volatile("int $0x0");
   asm volatile("int $0x3");
   asm volatile("int $0x9");
}
