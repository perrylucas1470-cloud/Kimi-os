#include "timer.h"
#include "../kernel/port.h"
#include "../kernel/vga.h"

static volatile uint32_t tick_count = 0;

void timer_init(uint32_t frequency) {
    // Calculate divisor
    uint32_t divisor = 1193180 / frequency;
    
    // Send command byte
    outb(0x43, 0x36);
    
    // Send divisor (low byte, then high byte)
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

void timer_handler(void) {
    tick_count++;
}

uint32_t timer_get_ticks(void) {
    return tick_count;
}

void sleep(uint32_t milliseconds) {
    uint32_t target = tick_count + (milliseconds * TIMER_FREQ / 1000);
    while (tick_count < target) {
        __asm__ volatile("hlt");
    }
}
