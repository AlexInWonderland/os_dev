#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
 
#include <kernel/tty.h>
#include <kernel/vga.h>
#include<kernel/common.h>
 
void kernel_early(void)
{
	terminal_initialize();
}
 
void kernel_main(void)
{

        for(size_t x=0 ; x < VGA_HEIGHT ; x++)
	   printf("Hello, kernel World!\n");
//	printf("Alex is here!\n");
//	printf("Alex is there!\r");
	printf("AAA\n");
	printf("BBB!!");
        //while(1);
}
