#ifndef _IDE_H_
#define _IDE_H_

#include "../common/types.h"
<<<<<<< HEAD
=======
#include "pio.h"
>>>>>>> e9e9c89319580cc1cf2d551b515c6bdd1a800275

#define SECTOR_SIZE 512

extern void read_sector(int sector, void *dst);
extern void write_sector(int sector, void *src);

#endif
