#include "timer.h"

static uint ticks;
static uint precounter;
static uint precounterTarget;

void timer_init(uint32_t freq_mhz) {
	if(freq_mhz > TIMER_HIGH_BOUND) {
		setColor(RED);
		printf("\nTIMER INITLIZATION ERROR : frequency too high\n");
		setColor(WHITE);
	}

	uint16_t load;
	int test = TIMER_HIGH_BOUND;
	for(load = 0; test > freq_mhz && load < TIMER_REGISTER_BOUND ; load++) {
		if(load % 2)
			test -= (FREQUENCY_STEP + 1);
		else
			test -= FREQUENCY_STEP;
	}

	if(test - freq_mhz > (test + FREQUENCY_STEP) - freq_mhz && load < TIMER_REGISTER_BOUND)
		load++;

	outw(TIMER_POINTER, load);
	precounter = 0;
	ticks = 0;
}

// void timer_init(uint32_t freq_hz) {	
// 	if(freq_mhz > TIMER_HIGH_BOUND) {
// 		setColor(RED);
// 		printf("\nTIMER INITLIZATION ERROR : frequency too high\n");
// 		setColor(WHITE);
// 	}

// 	outb(TIMER_POINTER, load);
// 	ticks = 0;
// 	precounter = 0;
// }

void timer_handler() {
	ticks++;
	setColor(GREEN);
	printf("\n.\n", ticks);
	setColor(WHITE);
}

uint get_ticks() {
	return ticks;
}

void sleep(uint ms) {

}
