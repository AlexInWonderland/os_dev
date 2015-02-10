HOST?=$(shell ../default-host.sh)
HOSTARCH:=$(shell ../target-triplet-to-arch.sh $(HOST))
 
CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=
CC=$(HOME)/opt/cross/bin/i686-elf-gcc
AS?=$(HOME)/opt/cross/bin/i686-elf-as 

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/
C_INCLUDE_PATH?= ../libc/include:./include/kernel
 
CFLAGS:=$(CFLAGS) -ffreestanding -fbuiltin -Wall -Wextra -m32
#CFLAGS:= $(CFLAGS) -ffreestanding -fbuiltin -Wall -Wextra -nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32
CPPFLAGS:=$(CPPFLAGS) -D__is_myos_kernel -I../libc/include -Iinclude
LDFLAGS:=$(LDFLAGS) -m32
LIBS:=$(LIBS) -L$(HOME)/os/myos/libc -lk -nostdlib -lgcc
 
ARCHDIR:=arch/$(HOSTARCH)
 
include $(ARCHDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)
 
OBJS:=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \
 
CRTI_OBJ:=$(ARCHDIR)/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)
CRTN_OBJ:=$(ARCHDIR)/crtn.o
BOOT_OBJ:=$(ARCHDIR)/boot.o
 
ALL_OUR_OBJS:=\
$(CRTI_OBJ) \
$(OBJS) \
$(CRTN_OBJ) \
 
OBJ_LINK_LIST:=\
$(CRTI_OBJ) \
$(OBJS) \
$(CRTN_OBJ) \
 
all: myos.kernel
 
.PHONY: all clean install install-headers install-kernel
 
myos.kernel: $(OBJ_LINK_LIST) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -m32 -o $@ $(CFLAGS) $(OBJ_LINK_LIST) $(LDFLAGS) $(LIBS)
 
#boot.o:boot.S
#	$(AS) -o $(BOOT_OBJ) boot.S	
%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
 
clean:
	rm -f myos.kernel $(OBJS) $(ALL_OUR_OBJS) *.o */*.o */*/*.o
 
install: install-headers install-kernel
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -Rv include $(DESTDIR)$(INCLUDEDIR)
 
install-kernel: myos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp myos.kernel $(DESTDIR)$(BOOTDIR)
