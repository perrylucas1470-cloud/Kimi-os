#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "shell.h"
#include "../drivers/keyboard.h"
#include "../drivers/timer.h"

void kernel_main(void) {
    // Initialize VGA first so we can see output
    vga_init();
    vga_setcolor(VGA_COLOR_LIGHT_GREEN | (VGA_COLOR_BLACK << 4));
    
    // Welcome banner
    vga_puts("========================================\n");
    vga_puts("        Welcome to Kimi-OS v0.2\n");
    vga_puts("        Built with GitHub Actions\n");
    vga_puts("========================================\n\n");
    
    vga_setcolor(VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4));
    
    // Initialize GDT
    vga_puts("[*] Initializing GDT... ");
    gdt_init();
    vga_setcolor(VGA_COLOR_LIGHT_GREEN | (VGA_COLOR_BLACK << 4));
    vga_puts("OK\n");
    vga_setcolor(VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4));
    
    // Initialize IDT
    vga_puts("[*] Initializing IDT... ");
    idt_init();
    vga_setcolor(VGA_COLOR_LIGHT_GREEN | (VGA_COLOR_BLACK << 4));
    vga_puts("OK\n");
    vga_setcolor(VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4));
    
    // Initialize ISRs and IRQs
    vga_puts("[*] Initializing ISRs... ");
    isr_init();
    vga_setcolor(VGA_COLOR_LIGHT_GREEN | (VGA_COLOR_BLACK << 4));
    vga_puts("OK\n");
    vga_setcolor(VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4));
    
    // Initialize timer
    vga_puts("[*] Initializing Timer (100Hz)... ");
    timer_init(TIMER_FREQ);
    register_interrupt_handler(32, timer_handler);
    vga_setcolor(VGA_COLOR_LIGHT_GREEN | (VGA_COLOR_BLACK << 4));
    vga_puts("OK\n");
    vga_setcolor(VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4));
    
    // Initialize keyboard
    vga_puts("[*] Initializing Keyboard... ");
    keyboard_init();
    register_interrupt_handler(33, keyboard_handler);
    vga_setcolor(VGA_COLOR_LIGHT_GREEN | (VGA_COLOR_BLACK << 4));
    vga_puts("OK\n");
    vga_setcolor(VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4));
    
    // Enable interrupts
    vga_puts("[*] Enabling Interrupts... ");
    __asm__ volatile("sti");
    vga_setcolor(VGA_COLOR_LIGHT_GREEN | (VGA_COLOR_BLACK << 4));
    vga_puts("OK\n\n");
    
    // Start shell
    vga_setcolor(VGA_COLOR_WHITE | (VGA_COLOR_BLACK << 4));
    shell_run();
    
    // Should never reach here
    while (1) {
        __asm__ volatile("hlt");
    }
}
