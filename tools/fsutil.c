#include "fsutil.h"

void readSuperblock(FILE* fp, superblock_t* sb) {
	fseek(fp, 0, SEEK_SET);
	fread(sb, SUPERBLOCK_SIZE, 1, fp);
}

uint8_t* readSector(int blocksize, int block, FILE* fp) {
	uint8_t* result = (uint8_t*) malloc(sizeof(char) * blocksize);
	fseek(fp, block * blocksize, SEEK_SET);
	fread(result, sizeof(uint8_t), blocksize, fp);
	return result;
}

inode_t* getInodeBlock(superblock_t superblock, int block, FILE* fp) {
	/*
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
	*/
	return NULL;
}

void getBitmapBlock(int blocksize, bitmap_t* bm, int block, FILE* fp) {
	uint8_t* bytes = readSector(blocksize, block, fp);
	*bm = (bitmap_t) {.bitmap = bytes};
}

int getFirstUnusedID(bitmap_t* bm, int blockSize) {
	int id = -1;
	for (int i = 0; i < blockSize; i++) {
		for (int j = 0; j < 8; j++) {
			if((bm->bitmap[i] & (1 << j)) == 0) {
				return i * 8 + j;
			}
		}
	}
	return id;
}

void writeSuperblock(superblock_t* sb, FILE* fp, int blockSize) {
	fwrite(sb, sizeof(char), SUPERBLOCK_SIZE, fp);
	//put zeros until we complete the block
	for(int i = 0; i < (blockSize-SUPERBLOCK_SIZE); i++) {
		fputc(0, fp);
	}
}

void writeInode(inode_t* inode, FILE* fp, int blockSize, float position) {
	fseek(fp, position * blockSize, SEEK_SET);
	fwrite(inode->name, sizeof(char), FILENAME_MAXSIZE, fp);
	fwrite(&(inode->blocks[0]), sizeof(uint32_t), DIRECT_BLOCK_COUNT, fp);
	fwrite(&(inode->indirectBlocks[0]), sizeof(uint32_t), INDIRECT_BLOCK_COUNT, fp);
	fwrite(&(inode->size), sizeof(uint32_t), 1, fp);
}

void writeBitmap(bitmap_t* bm, FILE* fp, int blockSize, int position) {
	fseek(fp, position * blockSize, SEEK_SET);
	fwrite(bm->bitmap, sizeof(char), blockSize, fp);
}

void writeAddress(int block, int offset, int blockSize, uint32_t adress, FILE* fp) {
	fseek(fp, block * blockSize + offset, SEEK_SET);
	fwrite(&adress, sizeof(uint32_t), 1, fp);
}
