#ifndef _TIMER_H_
#define _TIMER_H_

#include "../common/types.h"
#include "display.h"
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

/**
 * Initialize the timer
 * @param freq_hz the frequency at which the timer will run (in Hz)
 */
extern void timer_init(uint32_t freq_hz);

/**
 * Handles a timer tick interuption
 */
extern void timer_handler();

/**
 * Gets the number of tick of the timer
 * @return the number of ticks the timer has counted already
 */
extern uint get_ticks();

/**
 * Sleeps for a given amount of time
 * @param ms number of milliseconds to sleep
 */
extern void sleep(uint ms);

#endif
