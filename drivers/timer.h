#ifndef TIMER_H
#define TIMER_H

#include "../include/types.h"

#define TIMER_FREQ 100  // 100 Hz = 10ms ticks

void timer_init(uint32_t frequency);
void timer_handler(void);
uint32_t timer_get_ticks(void);
void sleep(uint32_t milliseconds);

#endif
