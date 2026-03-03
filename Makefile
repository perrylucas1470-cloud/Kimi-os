# Kimi-OS Makefile v0.2

CC = i686-elf-gcc
AS = nasm
LD = i686-elf-ld

CFLAGS = -g -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -c \
         -I. -Iinclude -std=gnu99

LDFLAGS = -T kernel/linker.ld -melf_i386

SRCDIR = .
BUILDDIR = build
ISODIR = $(BUILDDIR)/isodir

SOURCES = $(wildcard kernel/*.c libc/*.c drivers/*.c)
ASM_SOURCES = boot/boot.asm kernel/gdt_flush.asm kernel/idt_flush.asm kernel/isr_asm.asm

OBJECTS = $(patsubst %.c,$(BUILDDIR)/%.o,$(SOURCES))
ASM_OBJECTS = $(patsubst %.asm,$(BUILDDIR)/%.o,$(ASM_SOURCES))

TARGET = $(BUILDDIR)/kimi-os.bin
ISO = $(BUILDDIR)/kimi-os.iso

.PHONY: all clean iso run debug

all: $(TARGET)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(AS) -f elf32 $< -o $@

$(TARGET): $(OBJECTS) $(ASM_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

iso: $(ISO)

$(ISO): $(TARGET)
	mkdir -p $(ISODIR)/boot/grub
	cp $(TARGET) $(ISODIR)/boot/kimi-os.bin
	echo 'set timeout=0' > $(ISODIR)/boot/grub/grub.cfg
	echo 'set default=0' >> $(ISODIR)/boot/grub/grub.cfg
	echo '' >> $(ISODIR)/boot/grub/grub.cfg
	echo 'menuentry "Kimi-OS v0.2" {' >> $(ISODIR)/boot/grub/grub.cfg
	echo '    multiboot /boot/kimi-os.bin' >> $(ISODIR)/boot/grub/grub.cfg
	echo '    boot' >> $(ISODIR)/boot/grub/grub.cfg
	echo '}' >> $(ISODIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISODIR)

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 256 -serial stdio

run-debug: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 256 -serial stdio -s -S

clean:
	rm -rf $(BUILDDIR)
