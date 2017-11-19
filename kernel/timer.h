#ifndef _TIMER_H_
#define _TIMER_H_

#include <types.h>
#include <display.h>
#include "pio.h"

typedef struct timer_t {
	uint32_t ticks;
	uint32_t frequency;
} timer_t;

#define TIMER_HIGH_BOUND 1193180
#define TIMER_LOW_BOUND 18
#define TIMER_COMMAND_POINTER 0x43
#define TIMER_CHAN0_POINTER 0x40
#define TIMER_CHAN1_POINTER 0x41
#define TIMER_CHAN2_POINTER 0x42
#define TIMER_REGISTER_BOUND 65536
#define FREQUENCY_STEP 18206

void timer_init(uint32_t freq_hz);
void timer_handler();
uint get_ticks();
void sleep(uint ms);

#endif
