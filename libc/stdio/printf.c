#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
 
#if defined(__is_myos_kernel)
#include <kernel/tty.h>
#endif
 
static void print(const char* data, size_t data_length)
{
	for ( size_t i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}
 
int printf(const char* restrict format, ...)
{
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;
 
	while ( *format != '\0' )
	{
		if ( *format != '%' )
		{
		print_c:
			amount = 1;
			while ( format[amount] && format[amount] != '%' )
				amount++;
			print(format, amount);
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format;
 
		if ( *(++format) == '%' )
			goto print_c;
 
		if ( rejected_bad_specifier )
		{
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}
 
		if ( *format == 'c' )
		{
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			print(&c, sizeof(c));
		}
		else if ( *format == 's' )
		{
			format++;
			const char* s = va_arg(parameters, const char*);
			print(s, strlen(s));
		}
                else if (*format == 'i')  // hex
                {
                        format++;
                        int x = va_arg(parameters, int);
                        char c='0'; 
                        print(&c, sizeof(c));
                        c= 'x';
                        print(&c, sizeof(c));
                        if(x==0)
                        {
                           c='0';
                           print(&c, sizeof(c));
                           continue;
                        }
                        int i=0;
                        char c1[32];
                        while(x>0)
                        {
                           c1[i++] = x % 16;
                           x /= 16;
                        } 
                      //  i--;
                        while((--i) >= 0)
                        {
                          switch (c1[i])
                          {
                             case 0:
                             case 1:
                             case 2:
                             case 3:
                             case 4:
                             case 5:
                             case 6:
                             case 7:
                             case 8:
                             case 9:
                                c ='0' + c1[i];
                                print(&c, sizeof(c));
                                break;
                             case 10:
                                c='A';  
                                print(&c, sizeof(c));
                                break;
                             case 11:
                                c='B';
                                print(&c, sizeof(c));
                                break;
                             case 12:
                                c='C';
                                print(&c, sizeof(c));
                                break;
                             case 13:
                                c='D';
                                print(&c, sizeof(c));
                                break;
                             case 14:
                                c='E';
                                print(&c,sizeof(c));
                                break;
                             case 15:
                                c='F';
                                print(&c, sizeof(c));
                                break;
                          }
                          //i--;
                        }                                               
                }
                else if (*format == 'd' )   //print integer
                {
                       format++;
                       int x = va_arg(parameters, int);
                       if(x == 0)
                       {
                           char c='0';
                           print(&c,sizeof(c));
                           continue;
                       }
                       char c1[32];
                       int j1=0;
                       while(x > 0)
                       {
                          c1[j1++] = '0'+ (x%10);
                          x = x/10;   
                       }
                       //c1[j1]=0; 
            
                       char c2[32];
                       c2[j1--]=0;
                       int j2 = 0;
                       while(j1 >= 0)
                       {
                          c2[j2++] = c1[j1--];
                       } 
                       c2[j2]=0;
                       print(c2,strlen(c2));
                }
		else
		{
			goto incomprehensible_conversion;
		}
	}
 
	va_end(parameters);
 
	return written;
}
