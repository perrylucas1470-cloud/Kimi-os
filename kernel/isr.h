#ifndef ISR_H
#define ISR_H

#include "../include/types.h"

// ISR function type
typedef void (*isr_t)(void);

void isr_init(void);
void register_interrupt_handler(uint8_t n, isr_t handler);
void isr_handler(uint8_t int_no);

// IRQ numbers
#define IRQ0  32  // Timer
#define IRQ1  33  // Keyboard

#endif
