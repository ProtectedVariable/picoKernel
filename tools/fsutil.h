#ifndef __FSUTIL__
#define __FSUTIL__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define INODE_SIZE 512
#define SUPERBLOCK_SIZE 512
#define FILENAME_MAXSIZE 40
#define DIRECT_BLOCK_COUNT 113
#define INDIRECT_BLOCK_COUNT 4
#define SUPERBLOCK_MAGIC 0xCA11AB1E

#define FS_VERSION 1
#define INODE_BITMAP_OFFSET 1

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
	char label[490]; //long enough to reach 512 bytes
}__attribute__((packed)) superblock_t;

typedef struct inode_st {
	char name[FILENAME_MAXSIZE];
	uint32_t blocks[DIRECT_BLOCK_COUNT];
	uint32_t indirectBlocks[INDIRECT_BLOCK_COUNT];
	uint32_t size;
}__attribute__((packed)) inode_t;

typedef struct indirectBlock_st {
	uint32_t* addresses;
}__attribute__((packed)) indirectBlock_t;

typedef struct bitmap_st {
	uint8_t* bitmap;
}__attribute__((packed)) bitmap_t;

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
