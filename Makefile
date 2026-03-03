# Kimi-OS Makefile

CC = i686-elf-gcc
AS = nasm
LD = i686-elf-ld

CFLAGS = -g -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c \
         -I. -Iinclude

LDFLAGS = -T kernel/linker.ld -melf_i386

SRCDIR = .
BUILDDIR = build
ISODIR = $(BUILDDIR)/isodir

SOURCES = $(wildcard kernel/*.c libc/*.c drivers/*.c)
ASM_SOURCES = boot/boot.asm kernel/gdt_flush.asm kernel/idt_flush.asm

OBJECTS = $(patsubst %.c,$(BUILDDIR)/%.o,$(SOURCES))
ASM_OBJECTS = $(patsubst %.asm,$(BUILDDIR)/%.o,$(ASM_SOURCES))

TARGET = $(BUILDDIR)/kimi-os.bin
ISO = $(BUILDDIR)/kimi-os.iso

.PHONY: all clean iso run

all: $(TARGET)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(AS) -f elf32 $< -o $@

# Assembly helpers for GDT/IDT flushing
$(BUILDDIR)/kernel/gdt_flush.o: kernel/gdt_flush.asm
	@mkdir -p $(dir $@)
	$(AS) -f elf32 $< -o $@

$(BUILDDIR)/kernel/idt_flush.o: kernel/idt_flush.asm
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
	echo 'menuentry "Kimi-OS" {' >> $(ISODIR)/boot/grub/grub.cfg
	echo '    multiboot /boot/kimi-os.bin' >> $(ISODIR)/boot/grub/grub.cfg
	echo '    boot' >> $(ISODIR)/boot/grub/grub.cfg
	echo '}' >> $(ISODIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISODIR)

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 256

clean:
	rm -rf $(BUILDDIR)
