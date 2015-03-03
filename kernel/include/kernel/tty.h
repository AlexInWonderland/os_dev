#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H
 
#include <stddef.h>

#define TTY_BUFFER_SIZE 256
#define NR_CONSOLES 3
//TTY tty_table[NR_CONSOLES];
#define TTY_FIRST (tty_table)
#define TTY_END   (tty_table+NR_CONSOLES)

struct s_console;

typedef struct s_tty
{
    unsigned int in_buf[TTY_BUFFER_SIZE];
    unsigned int* p_inbuf_head;
    unsigned int* p_inbuf_tail;
    int inbuf_count;
  
    struct s_console* p_console;

}TTY;

TTY tty_table[NR_CONSOLES];

static void init_tty(TTY* p_tty);
static void tty_do_read(TTY* p_tty);
static void tty_do_write(TTY* p_tty);
void in_process(TTY* p_tty, unsigned int key);
void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

#endif
