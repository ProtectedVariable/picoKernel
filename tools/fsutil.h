#ifndef __FSUTIL__
#define __FSUTIL__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INODE_SIZE 512
#define SUPERBLOCK_SIZE 512
#define FILENAME_MAXSIZE 40
#define DIRECT_BLOCK_COUNT 113
#define INDIRECT_BLOCK_COUNT 4

typedef struct superblock_st {
	uint32_t magic;
	uint32_t version;
	uint16_t blockSize;
	uint32_t inodeCount;
	uint8_t inodeBitmapSize;
	uint16_t inodeBitmapOffset;
	uint8_t dataBitmapSize;
	uint16_t dataBitmapOffset;
	uint16_t inodeList;
	char* label; //long enough to reach 512 bytes
} superblock_t;

typedef struct inode_st {
	char name[FILENAME_MAX];
	uint32_t blocks[DIRECT_BLOCK_COUNT];
	uint32_t indirectBlocks[INDIRECT_BLOCK_COUNT];
	uint32_t size;
} inode_t;

typedef struct bitmap_st {
	uint8_t* bitmap;
} bitmap_t;

extern superblock_t readSuperblock(FILE* fp);
extern uint8_t* readSector(superblock_t superblock, int block, FILE* fp);
extern inode_t* getInodeBlock(superblock_t superblock, int block, FILE* fp);
extern bitmap_t getBitmapBlock(superblock_t superblock, int block, FILE* fp);

#endif
