#include "vga.h"
#include "../libc/string.h"
#include <stdarg.h>

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
    vga_update_cursor();
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
        vga_scroll();
    }
    
    vga_update_cursor();
}

void vga_puts(const char* str) {
    while (*str) {
        vga_putchar(*str++);
    }
}

void vga_scroll(void) {
    // Move everything up one line
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

void vga_update_cursor(void) {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

// Simple number to string conversion
static void itoa(int value, char* str, int base) {
    char* ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;
    
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);
    
    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    
    // Reverse string
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

void vga_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    char buffer[32];
    char* str;
    int num;
    
    while (*format) {
        if (*format == '%' && *(format + 1)) {
            format++;
            switch (*format) {
                case 's':
                    str = va_arg(args, char*);
                    vga_puts(str ? str : "(null)");
                    break;
                case 'd':
                case 'i':
                    num = va_arg(args, int);
                    itoa(num, buffer, 10);
                    vga_puts(buffer);
                    break;
                case 'x':
                case 'X':
                    num = va_arg(args, int);
                    itoa(num, buffer, 16);
                    vga_puts("0x");
                    vga_puts(buffer);
                    break;
                case 'c':
                    vga_putchar((char)va_arg(args, int));
                    break;
                case '%':
                    vga_putchar('%');
                    break;
                default:
                    vga_putchar('%');
                    vga_putchar(*format);
                    break;
            }
        } else {
            vga_putchar(*format);
        }
        format++;
    }
    
    va_end(args);
}
