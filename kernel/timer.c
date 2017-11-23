/* Timer source file
 * Ibanez Thomas, Tournier Vincent
 */
#include "timer.h"

static timer_t timer;

void timer_init(uint32_t freq_hz) {
	if(freq_hz > TIMER_HIGH_BOUND) {
		setColor(RED);
		printf("\nTIMER INITLIZATION ERROR : frequency too high\n");
		setColor(WHITE);
	} else if(freq_hz < TIMER_LOW_BOUND) {
		setColor(RED);
		printf("\nTIMER INITLIZATION ERROR : frequency too low\n");
		setColor(WHITE);
	}

	uint16_t divisor = TIMER_HIGH_BOUND / freq_hz;

	timer.ticks = 0;
	timer.frequency = freq_hz;
	outb(TIMER_COMMAND_POINTER, 0x36);
	outb(TIMER_CHAN0_POINTER, divisor & 0xFF); //LSB first
	outb(TIMER_CHAN0_POINTER, divisor >> 8); //MSB
}

void timer_handler() {
	timer.ticks++;
}

uint get_ticks() {
	return timer.ticks;
}

void sleep(uint ms) {
	uint32_t end = timer.ticks + ((ms * timer.frequency) / 1000);
	while(timer.ticks != end) {} //wait
}
