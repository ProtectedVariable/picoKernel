#ifndef __FSUTIL__
#define __FSUTIL__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../kernel/fsdata.h"

extern void readSuperblock(FILE* fp, superblock_t* sb);
extern uint8_t* readSector(int blocksize, int block, FILE* fp);
extern void getInodeBlock(int blockSize, int blockID, int inodeList, inode_t* inode, FILE* fp) ;
extern void getBitmapBlock(int blocksize, bitmap_t* bm, int block, FILE* fp);

extern int getFirstUnusedID(bitmap_t* bm, int blockSize);

extern void writeSuperblock(superblock_t* sb, FILE* fp, int blockSize);
extern void writeBitmap(bitmap_t* bm, FILE* fp, int blockSize, int position);

extern void writeInode(inode_t* inode, FILE* fp, int blockSize, float position);
extern void writeAddress(int block, int offset, int blockSize, uint32_t adress, FILE* fp);
uint32_t readAddress(int blockID, int offset, int blockSize, FILE* fp);

extern void allocBlock(int bitmapCount, int bitmapOffset, int blockSize, FILE* diskFile, int* id);
extern void freeBlock(int dataBitmap, int blockSize, int blockID, FILE* fp);

#endif
