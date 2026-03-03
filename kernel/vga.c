#include "vga.h"

static uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;
static uint8_t vga_color = VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4);
static int cursor_x = 0;
static int cursor_y = 0;

static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

void vga_init(void) {
    vga_clear();
}

void vga_clear(void) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = vga_entry(' ', vga_color);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

void vga_setcolor(uint8_t color) {
    vga_color = color;
}

void vga_putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\t') {
        cursor_x = (cursor_x + 4) & ~3;
    } else {
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c, vga_color);
        cursor_x++;
    }
    
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= VGA_HEIGHT) {
        // Scroll up
        for (int y = 0; y < VGA_HEIGHT - 1; y++) {
            for (int x = 0; x < VGA_WIDTH; x++) {
                vga_buffer[y * VGA_WIDTH + x] = vga_buffer[(y + 1) * VGA_WIDTH + x];
            }
        }
        // Clear last line
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', vga_color);
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}

void vga_puts(const char* str) {
    while (*str) {
        vga_putchar(*str++);
    }
}

// Simple printf implementation
void vga_printf(const char* format, ...) {
    // Simplified - just print the string for now
    vga_puts(format);
}
