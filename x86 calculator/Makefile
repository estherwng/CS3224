USE_NASM = false

ifndef TOOLPREFIX
TOOLPREFIX := $(shell if i386-jos-elf-objdump -i 2>&1 | grep '^elf32-i386$$' >/dev/null 2>&1; \
	then echo 'i386-jos-elf-'; \
	elif objdump -i 2>&1 | grep 'elf32-i386' >/dev/null 2>&1; \
	then echo ''; \
	else echo "***" 1>&2; \
	echo "*** Error: Couldn't find an i386-*-elf version of GCC/binutils." 1>&2; \
	echo "*** Is the directory with i386-jos-elf-gcc in your PATH?" 1>&2; \
	echo "*** If your i386-*-elf toolchain is installed with a command" 1>&2; \
	echo "*** prefix other than 'i386-jos-elf-', set your TOOLPREFIX" 1>&2; \
	echo "*** environment variable to that prefix and run 'make' again." 1>&2; \
	echo "*** To turn off this error, run 'gmake TOOLPREFIX= ...'." 1>&2; \
	echo "***" 1>&2; exit 1; fi)
endif

ifeq ($(USE_NASM), true)
	CC = nasm
else
	CC = $(TOOLPREFIX)gcc
endif
LD = $(TOOLPREFIX)ld
OBJCOPY = $(TOOLPREFIX)objcopy
QEMU = qemu-system-i386

ifeq ($(USE_NASM), true)
	CFLAGS = -f elf
else
	CFLAGS = -m32 -ffreestanding -fno-PIE -c
endif

LDFLAGS = -melf_i386 -T link.ld
QEMUFLAGS = -display none -nographic -fda

%.o: %.s
	$(CC) $(CFLAGS) $^ -o $@

%.elf: %.o
	$(LD) $(LDFLAGS) $^ -o $@

%.bin: %.elf
	$(OBJCOPY) -O binary $^ $@

qemu-%: %.bin
	$(QEMU) $(QEMUFLAGS) $^

clean:
	rm -f *.o *.elf *.bin
