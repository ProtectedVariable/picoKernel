#ifndef _TIMER_H_
#define _TIMER_H_

#include <types.h>
#include <display.h>
#include "pio.h"

#define TIMER_HIGH_BOUND 1193180000
#define TIMER_LOW_BOUND 18206
#define TIMER_POINTER 0x43
#define TIMER_REGISTER_BOUND 65536
#define FREQUENCY_STEP 18206

void timer_init(uint32_t freq_hz);
void timer_handler();
uint get_ticks();
void sleep(uint ms);

#endif
