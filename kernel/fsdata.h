#ifndef _FS_DATA_H_
#define _FS_DATA_H_

#include "../common/types.h"
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
	uint32_t inodeMax;
	uint8_t inodeBitmapSize;
	uint16_t inodeBitmapOffset;
	uint8_t dataBitmapSize;
	uint16_t dataBitmapOffset;
	uint16_t inodeList;
	uint32_t inodesCount;
	uint32_t dataBlockOffset;
	uint32_t dataBlockCount;
	uint32_t dataBlockUsed;
	char label[474]; //long enough to reach 512 bytes
}__attribute__((packed)) superblock_t;

typedef struct inode_st {
	char name[FILENAME_MAXSIZE];
	uint32_t blocks[DIRECT_BLOCK_COUNT];
	uint32_t indirectBlocks[INDIRECT_BLOCK_COUNT];
	uint32_t exactSize;
}__attribute__((packed)) inode_t;

typedef struct indirectBlock_st {
	uint32_t* addresses;
}__attribute__((packed)) indirectBlock_t;

typedef struct bitmap_st {
	uint8_t* bitmap;
}__attribute__((packed)) bitmap_t;


#endif /* end of include guard: _FS_DATA_H_ */
