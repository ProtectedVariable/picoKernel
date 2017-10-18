#ifndef _PIO_H_
#define _PIO_H_
#include <stdint.h>

extern void outb(uint16_t port, uint8_t data);
extern void outw(uint16_t port, uint16_t data);
extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);

#endif /* end of include guard: _PIO_H_ */
