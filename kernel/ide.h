#ifndef _IDE_H_
#define _IDE_H_

#include "../common/types.h"

#define SECTOR_SIZE 512

extern void read_sector(int sector, void *dst);
extern void write_sector(int sector, void *src);

#endif
