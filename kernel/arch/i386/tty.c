#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
 
#include <kernel/vga.h>
#include <kernel/common.h>
#include <kernel/tty.h>
 
size_t cury;
size_t curx;
uint8_t terminal_color;
uint16_t* fb;    //frame buffer

static void move_cursor();
static void scroll();
 
void terminal_initialize(void)
{
	cury=0;
	curx=0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	fb=VGA_MEMORY;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
	    for ( size_t x = 0; x < VGA_WIDTH; x++ )
	    {
		const size_t index = y * VGA_WIDTH + x;
	    	fb[index]=make_vgaentry(' ', terminal_color); 
	    }
	}
	move_cursor();
}
 
void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	fb[index]=make_vgaentry(c, color);
}
 
void terminal_putchar(char c)
{
	if (c==0x08 && curx)   //back space
	{  
	    curx--;
	}
        else if (c==0x09)
	{
	    curx = (curx+8) & ~(8-1);
	}
	else if (c=='\r')
	{
	    curx = 0;
	}
	else if (c=='\n')
	{
 	    curx=0;
	    cury++;
            if(cury >= VGA_HEIGHT)
 	        scroll();
	}
	else if(c>=' ')
	{
	  terminal_putentryat(c, terminal_color, curx, cury);
	  curx++;
	}
        scroll();
	move_cursor();
}
 
void terminal_write(const char* data, size_t size)
{
	for ( size_t i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}
 
void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

/*---scroll the terminal---*/
static void scroll()
{
    if(cury >= VGA_HEIGHT)
    {
	for(size_t row = 0 ; row < VGA_HEIGHT ; row++ )
	{
	    for(size_t col = 0; col < VGA_WIDTH; col++ )
	    {
		size_t index1 = row * VGA_WIDTH + col;
		size_t index2 = (row + 1)* VGA_WIDTH + col;
	        fb[index1]=fb[index2];
	    }
	 }
	 cury--;
	 curx = 0;
    }	
} 

/*---move the cursor----*/
static void move_cursor()
{
    uint16_t cLoc = cury*80 + curx;
    outb(0x3D4, 14);
    outb(0x3D5, cLoc >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cLoc);
}
