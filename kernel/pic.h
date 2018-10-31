/* PIC header file
 * Ibanez Thomas, Tournier Vincent
 */
#ifndef _PIC_H_
#define _PIC_H_

#include "pio.h"

/**
 * Initialize the PIC
 */
extern void pic_init();

/**
 * Finalize an interruption
 * @param irq the interruption to finalize
 */
extern void pic_eoi(int irq);

#endif
