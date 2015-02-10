.global gdt_flush
.extern gdt_ptr

gdt_flush:
	lgdt gdt_ptr        # Load the GDT with our '_gp' which is a special pointer
	mov $0x10, %ax      # 0x10 is the offset in the GDT to our data segment
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	#jump to an 0x08 offset from flush2.
	jmp $0x08,$flush2
flush2:
	ret
