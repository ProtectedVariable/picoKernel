#ifndef _TIMER_H_
#define _TIMER_H_

extern void timer_init(uint32_t freq_hz);
extern void timer_handler();
extern uint get_ticks();
extern void sleep(uint ms);

#endif
