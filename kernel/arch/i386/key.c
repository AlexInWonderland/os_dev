#include <stdio.h>

#include <kernel/common.h>
#include <kernel/irq.h>
#include <kernel/isr.h>
#include <kernel/tty.h>
#include <kernel/keymap.h>
#include <kernel/key.h>

static int code_with_E0 = 0;
static int shift_l;
static int shift_r;
static int alt_l;
static int alt_r;
static int ctrl_l;
static int ctrl_r;
static int caps_lock;
static int num_lock;
static int scroll_lock;
static int column;

static KB_INPUT kb_in;    //buffer
/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r)
{
    unsigned char scan_code;

    /* Read from the keyboard's data buffer */
    scan_code = inb(0x60);
    if(kb_in.count < KB_IN_BYTES)
    { 
         *(kb_in.p_head) = scan_code;
         kb_in.p_head++;
         if(kb_in.p_head == kb_in.buf + KB_IN_BYTES)
         {
              kb_in.p_head = kb_in.buf;
         }  
         kb_in.count++;
    }
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_install()
{
    kb_in.count=0;
    kb_in.p_head=kb_in.p_tail = kb_in.buf;
    irq_install_handler(1, keyboard_handler);
    asm volatile("sti");  //enable interrupt
}

void keyboard_read(TTY* p_tty)
{
   unsigned char scan_code;
   int make;
   unsigned int key = 0;
   
 //  char output[2];
 
   unsigned int *keyrow;   

   if(kb_in.count > 0)
   {
	code_with_E0 = 0;
        scan_code = get_byte_from_kbuf();
        if (scan_code == 0xE0)
        {
            scan_code=get_byte_from_kbuf();
            if(scan_code == 0x2A)  /*print screen pressed*/
            {
                if(get_byte_from_kbuf() == 0xE0)
                {
                    if(get_byte_from_kbuf()==0x37)
                    {
                         key=PRINTSCREEN;
                         make=1;
                    }
                }
            }
            /*print screen released*/
            if(scan_code == 0xB7)
            {
                 if(get_byte_from_kbuf() == 0xE0)
                 {
                      if(get_byte_from_kbuf() == 0xAA)
                      {
                          key=PRINTSCREEN;
                          make = 0; 
                      }
                 }
            }
            if(key == 0)
	        code_with_E0 = 1;
        }
        else if(scan_code == 0xE1)   //for pause break
        {
            int i;   
            unsigned char pausebrk_scancode[] = { 0xE1, 0x1D, 0x45, 
                                                  0xE1, 0x9D, 0xC5};
            int is_pause_brk = 1;
            for( i=1 ; i<6 ; i++)
            {
                 if(get_byte_from_kbuf()!=pausebrk_scancode[i])
                 {
                      is_pause_brk = 0;
                      break;
                 }    
            }
            if(is_pause_brk)
                 key=PAUSEBREAK;
        }
        if((key!=PAUSEBREAK) && (key!=PRINTSCREEN))
        {
             make = (scan_code & 0x0080 ? 0:1);   //make=1 make code 0 break code
             keyrow = &keymap[(scan_code & 0x7F)*MAP_COLS];
            
             column = 0;
             if(shift_l || shift_r)
                 column = 1;
             if(code_with_E0)
             {
                 column = 2;
                 code_with_E0 = 0;
             }  
                
             key = keyrow[column];
           //  printf("%d ",key);
           //  printf("%d ",column);
           //  printf("%d ",scan_code);
             switch (key)
             {
                case SHIFT_L:
                     shift_l = make;
                     break;  
                case SHIFT_R:
                     shift_r = make;
                     break;
                case CTRL_L:
                     ctrl_l = make;
                     break;
                case CTRL_R:
                     ctrl_r = make;
                     break;
                case ALT_L:
                     alt_l = make;
                     break; 
                case ALT_R:
                     alt_r = make;
                     break;
                default:
                     break;
             } 
             //printf("%d",scan_code);
             if(make)
             {
                  key|= shift_l ? FLAG_SHIFT_L : 0;
                  key|= shift_r ? FLAG_SHIFT_R : 0;
                  key|= ctrl_l  ? FLAG_CTRL_L  : 0;
                  key|= ctrl_r  ? FLAG_CTRL_R  : 0; 
                  key|= alt_l   ? FLAG_ALT_L   : 0;
                  key|= alt_r   ? FLAG_ALT_R   : 0;
                  
                  in_process(p_tty, key);   
                 // printf("%d", key);
             }       
             /*if(make)
             {
                   output[0]= keymap[(scan_code&0x7F)*MAP_COLS];
                   printf("%s",output);
             }*/
        }	 
   }
}

/*read one byte from kbuf*/
static unsigned char get_byte_from_kbuf()
{
    unsigned char scan_code;
    while (kb_in.count <= 0){}
    
    asm volatile("cli");  
    scan_code = *(kb_in.p_tail);
    kb_in.p_tail++;
    if(kb_in.p_tail == kb_in.buf + KB_IN_BYTES)
        kb_in.p_tail = kb_in.buf;
    kb_in.count--;
    asm volatile("sti");
    return scan_code;
}

/*static void in_process(unsigned int key)
{
      char output[2] = {'\0','\0'};
      if(!(key & FLAG_EXT)){   //non-special key
             output[0] = key & 0xFF;
             printf("%s",output);
             //printf("%d",output);
      }
      else
      {
         printf("AAA");
         int raw_code = key & 0x01FF;
         switch (raw_code)
         { 
            case UP:
                    if((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
                    {
                          asm volatile("cli");
                          outb(0x3D4, 0xC);
                          outb(0x3D5, ((80*15) >> 8) & 0xFF);
                          outb(0x3D4, 0xD);
                          outb(0x3D5, (80*15) & 0xFF); 
                          asm volatile("sti");
                    }
                    break;
            case DOWN:
                 break;  
            default:
                 break; 
         }
      }
}
*/
