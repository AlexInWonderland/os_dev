#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
 
#include <kernel/vga.h>
#include <kernel/common.h>
#include <kernel/tty.h>
#include <kernel/console.h>
#include <kernel/global.h>
#include <kernel/key.h>
 
size_t cury;
size_t curx;
uint8_t terminal_color;
uint16_t* fb;    //frame buffer

static void move_cursor();
static void scroll();

void task_tty()   //for testing keyboard
{
	TTY* p_tty;
       // keyboard_install();
        for(p_tty=TTY_FIRST; p_tty < TTY_END; p_tty++)
        {
              init_tty(p_tty);
        }
       // nr_current_console = 0;
        select_console(0);
        while(1)
        {
            for(p_tty=TTY_FIRST;p_tty<TTY_END;p_tty++)
            {
                    tty_do_read(p_tty);
                    tty_do_write(p_tty);
            }
        }
}

static void tty_do_write(TTY* p_tty)
{
	if (p_tty->inbuf_count) {
		char ch = *(p_tty->p_inbuf_tail);
		p_tty->p_inbuf_tail++;
		if (p_tty->p_inbuf_tail == p_tty->in_buf + TTY_BUFFER_SIZE) {
			p_tty->p_inbuf_tail = p_tty->in_buf;
		}
		p_tty->inbuf_count--;
		out_char(p_tty->p_console, ch);
	}
}
 
static void init_tty(TTY* p_tty)
{
     p_tty->inbuf_count = 0;
     //disp_pos =0;
     p_tty->p_inbuf_head=p_tty->p_inbuf_tail=p_tty->in_buf;
     
    // int nr_tty = p_tty - tty_table;
     //printf(" nr_tty: %d", nr_tty);
     //printf(" console_table %d", console_table);
     //p_tty->p_console = console_table + nr_tty;
     init_screen(p_tty);
}

static void tty_do_read(TTY* p_tty)
{
    if(is_current_console(p_tty->p_console))
    {
         keyboard_read(p_tty);
    }
}

void in_process(TTY* p_tty, unsigned int key)
{
        char output[2] = {'\0', '\0'};

        if (!(key & FLAG_EXT)) {
		if (p_tty->inbuf_count < TTY_BUFFER_SIZE) {
		    *(p_tty->p_inbuf_head) = key;
		    p_tty->p_inbuf_head++;
		    if (p_tty->p_inbuf_head == p_tty->in_buf + TTY_BUFFER_SIZE){
			p_tty->p_inbuf_head = p_tty->in_buf;
		    }
		    p_tty->inbuf_count++;
		}
        }
        else {
                int raw_code = key & 0x01FF;
                switch(raw_code) {
                case UP:
                        if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
                                asm volatile("cli");
                                outb(0x3D4, 0xC);
                                outb(0x3D5, ((80*15) >> 8) & 0xFF);
                                outb(0x3D4, 0xD);
                                outb(0x3D5, (80*15) & 0xFF);
                                asm volatile("sti");
                        }
                        break;
                case DOWN:
                        if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
				/* Shift+Down, do nothing */
                        }
                        break;
                case F1:
                case F2:
                case F3:
                case F4:
                case F5:
                case F6:
                case F7:
                case F8:
                case F9:
                case F10:
                case F11:
                case F12:
                    if((key & FLAG_ALT_L)|| (key & FLAG_ALT_R))
                    {
                        select_console(raw_code-F1);
                    }
                    break;  
                default:
                        break;
                }
        }
}

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
