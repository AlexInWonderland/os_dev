//adapted from James Molloy's tutorial
#include<stdint.h>

#include <kernel/common.h>
#include <kernel/gdt.h>

extern void gdt_flush();
//extern void _gdt_flush(uint32_t);

void init_gdt();
static void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;
//idt_entry_t idt_entries[256];
//idt_ptr_t idt_ptr;

void init_descriptor_tables()
{
  init_gdt();
}

void init_gdt()
{
   gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
   gdt_ptr.base = (uint32_t) & gdt_entries;
 
   gdt_set_gate(0,0,0,0,0);
   gdt_set_gate(1,0,0xFFFFFFFF, 0x9A, 0xCF);
   gdt_set_gate(2,0,0xFFFFFFFF, 0x92, 0xCF);
   gdt_set_gate(3,0,0xFFFFFFFF, 0xFA, 0xCF);
   gdt_set_gate(4,0,0xFFFFFFFF, 0xF2, 0xCF);  
   
  // _gdt_flush((uint32_t)&gdt_ptr);   
   gdt_flush();
}

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low     = (base & 0xFFFF);        
    gdt_entries[num].base_middle  = (base >> 16) & 0xFF;
    gdt_entries[num].base_high    = (base >> 24) & 0xFF;
   
    gdt_entries[num].limit_low    = limit & 0xFFFF;     
    gdt_entries[num].granularity  = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity  |= gran & 0xF0;
    gdt_entries[num].access       = access;
}
