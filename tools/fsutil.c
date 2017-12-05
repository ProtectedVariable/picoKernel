#include "fsutil.h"

superblock_t readSuperblock(FILE* fp) {
	uint8_t bytes[SUPERBLOCK_SIZE];
	fseek(fp, 0, SEEK_SET);
	fread(bytes, sizeof(char), SUPERBLOCK_SIZE, fp);
	superblock_t superblock = (superblock_t) {
		.magic = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3],
		.version = (bytes[4] << 24) | (bytes[5] << 16) | (bytes[6] << 8) | bytes[7],
		.blockSize = (bytes[8] << 8) | bytes[9],
		.inodeCount = (bytes[10] << 24) | (bytes[11] << 16) | (bytes[12] << 8) | bytes[13],
		.inodeBitmapSize = bytes[14],
		.inodeBitmapOffset = (bytes[15] << 8) | bytes[16],
		.dataBitmapSize = bytes[17],
		.dataBitmapOffset = (bytes[18] << 8) | bytes[19],
		.inodeList = (bytes[20] << 8) | bytes[21],
		.label = (char*) &bytes[22]
	};

	return superblock;
}

uint8_t* readSector(superblock_t superblock, int block, FILE* fp) {
	uint8_t* result = (uint8_t*) malloc(sizeof(char) * superblock.blockSize);
	fseek(fp, block * superblock.blockSize, SEEK_SET);
	fread(result, sizeof(uint8_t), superblock.blockSize, fp);
	return result;
}

inode_t* getInodeBlock(superblock_t superblock, int block, FILE* fp) {
	int inodeCount = superblock.blockSize / INODE_SIZE;
	inode_t* result = (inode_t*) malloc(sizeof(inodeCount * sizeof(inode_t)));
	uint8_t* bytes = readSector(superblock, block, fp);
	for(int i = 0 ; i < inodeCount ; i++) {
		char filename[FILENAME_MAXSIZE];
		uint32_t direct[DIRECT_BLOCK_COUNT];
		uint32_t indirect[INDIRECT_BLOCK_COUNT];
		for(int j = 0 ; j < FILENAME_MAXSIZE ; j++) {
			filename[j] = bytes[(i * INODE_SIZE) + j];
		}
		for(int j = 0 ; j < DIRECT_BLOCK_COUNT ; j += 4) {
			direct[j / 4] = (bytes[(i * INODE_SIZE) + FILENAME_MAXSIZE + j] << 24) |
			(bytes[(i * INODE_SIZE) + FILENAME_MAXSIZE + j + 1] << 16) |
			(bytes[(i * INODE_SIZE) + FILENAME_MAXSIZE + j + 2] << 8) |
			bytes[(i * INODE_SIZE) + FILENAME_MAXSIZE + j + 3];
		}
		for(int j = 0 ; j < INDIRECT_BLOCK_COUNT ; j += 4) {
			indirect[j / 4] = (bytes[(i * INODE_SIZE) + FILENAME_MAXSIZE + DIRECT_BLOCK_COUNT + j] << 24) |
				(bytes[(i * INODE_SIZE) + FILENAME_MAXSIZE + DIRECT_BLOCK_COUNT + j + 1] << 16) |
				(bytes[(i * INODE_SIZE) + FILENAME_MAXSIZE + DIRECT_BLOCK_COUNT + j + 2] << 8) |
				bytes[(i * INODE_SIZE) + FILENAME_MAXSIZE + DIRECT_BLOCK_COUNT + j + 3];
		}
		result[i] = (inode_t) {
			.name = "",
			.size = (bytes[INODE_SIZE - 4] << 24) |
				(bytes[INODE_SIZE - 3] << 16) |
				(bytes[INODE_SIZE - 2] << 8) |
				bytes[INODE_SIZE - 1],
			.blocks = {0},
			.indirectBlocks = {0}
		};
		memcpy(result[i].name, filename, sizeof(filename));
		memcpy(result[i].blocks, direct, sizeof(direct));
		memcpy(result[i].indirectBlocks, indirect, sizeof(indirect));
	}

	free(bytes);
	return result;
}

bitmap_t getBitmapBlock(superblock_t superblock, int block, FILE* fp) {
	uint8_t* bytes = readSector(superblock, block, fp);
	bitmap_t result = (bitmap_t) {.bitmap = bytes};
	free(bytes);
	return result;
}
