#include "vga.h"
#include "gdt.h"
#include "idt.h"

void kernel_main(void) {
    // Initialize VGA
    vga_init();
    vga_setcolor(VGA_COLOR_LIGHT_GREEN | (VGA_COLOR_BLACK << 4));
    
    // Welcome message
    vga_puts("========================================\n");
    vga_puts("        Welcome to Kimi-OS v0.1\n");
    vga_puts("        Built with GitHub Actions\n");
    vga_puts("========================================\n\n");
    
    vga_setcolor(VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4));
    vga_puts("Initializing GDT... ");
    gdt_init();
    vga_puts("[OK]\n");
    
    vga_puts("Initializing IDT... ");
    idt_init();
    vga_puts("[OK]\n");
    
    vga_puts("\nSystem ready. This is a bare-bones OS kernel.\n");
    vga_puts("Running in 32-bit protected mode.\n\n");
    
    vga_setcolor(VGA_COLOR_LIGHT_CYAN | (VGA_COLOR_BLACK << 4));
    vga_puts("Commands: None yet! This is just the beginning.\n");
    
    // Hang - we don't have multitasking yet
    while (1) {
        __asm__ volatile ("hlt");
    }
}
