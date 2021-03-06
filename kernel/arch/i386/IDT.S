# Loads the IDT defined in 'idtp' into the processor.
# This is declared in C as 'extern void idt_load();'
# based off of http://www.osdever.net/bkerndev/Docs/idt.htm
.global idt_load
.extern idt_ptr
idt_load:
	#loads the IDT table. idtp is the IDT pointer.
	lidt idt_ptr
	ret
