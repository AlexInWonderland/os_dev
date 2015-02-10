//adapted from James molloy tutorial
#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

void init_gdt();
struct gdt_entry_struct
{
   uint16_t limit_low;
   uint16_t base_low;
   uint8_t base_middle;
   uint8_t access;
   uint8_t granularity;
   uint8_t base_high;
}__attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct
{
   uint16_t limit;
   uint32_t base;
}__attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

void init_descriptor_tables();
#endif