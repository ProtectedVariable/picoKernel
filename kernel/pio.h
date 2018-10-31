/* PIO header file
 * Ibanez Thomas, Tournier Vincent
 */
#ifndef _PIO_H_
#define _PIO_H_
#include <stdint.h>

/**
 * Writes 8 bits of data to a given pio port
 * @param port port to write to
 * @param data data to write
 */
extern void outb(uint16_t port, uint8_t data);

/**
 * Writes 16 bits of data to a given pio port
 * @param port port to write to
 * @param data data to write
 */
extern void outw(uint16_t port, uint16_t data);

/**
 * Reads 8 bits from a pio port
 * @param  port port to read from
 * @return      data read
 */
extern uint8_t inb(uint16_t port);

/**
 * Reads 16 bits from a pio port
 * @param  port port to read from
 * @return      data read
 */
extern uint16_t inw(uint16_t port);

#endif /* end of include guard: _PIO_H_ */
