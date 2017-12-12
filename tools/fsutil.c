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

void getInodeBlock(int blockSize, int blockID, int inodeList, inode_t* inode, FILE* fp) {
	fseek(fp, inodeList * blockSize + (blockID-inodeList) * INODE_SIZE, SEEK_SET);
	fread(inode->name, sizeof(char), FILENAME_MAXSIZE, fp);
	fread(inode->blocks, sizeof(uint32_t), DIRECT_BLOCK_COUNT, fp);
	fread(inode->indirectBlocks, sizeof(uint32_t), INDIRECT_BLOCK_COUNT, fp);
	fread(&(inode->size), sizeof(uint32_t), 1, fp);
	fread(&(inode->exactSize), sizeof(uint32_t), 1, fp);
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
	fseek(fp, 0, SEEK_SET);
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
	fwrite(&(inode->exactSize), sizeof(uint32_t), 1, fp);
}

void writeBitmap(bitmap_t* bm, FILE* fp, int blockSize, int position) {
	fseek(fp, position * blockSize, SEEK_SET);
	fwrite(bm->bitmap, sizeof(char), blockSize, fp);
}

void writeAddress(int block, int offset, int blockSize, uint32_t adress, FILE* fp) {
	fseek(fp, block * blockSize + offset, SEEK_SET);
	fwrite(&adress, sizeof(uint32_t), 1, fp);
}

void allocBlock(int bitmapCount, int bitmapOffset, int blockSize, FILE* diskFile, int* id) {
    *id = -1;
    int bitmapPosition = 0;
    bitmap_t bm;
    for (int i = 0; i < bitmapCount; i++) {
        getBitmapBlock(blockSize, &bm, i + bitmapOffset, diskFile);
        *id = getFirstUnusedID(&bm, blockSize);
        if(*id != -1) {
            bitmapPosition = i + bitmapOffset;
            break;
        }
    }

    if(*id == -1) {
        printf("Disk Full !\n");
    }
    bm.bitmap[*id / 8] |= (1 << (*id % 8));
    writeBitmap(&bm, diskFile, blockSize, bitmapPosition);
}

uint32_t readAddress(int blockID, int offset, int blockSize, FILE* fp) {
	fseek(fp, blockID * blockSize + offset, SEEK_SET);
	uint32_t a;
	fread(&a, sizeof(uint32_t), 1, fp);
	return a;
}

void freeBlock(int bitmap, int blockSize, int blockID, FILE* fp) {
	int bitmapID = blockID / (blockSize * 8);
	bitmap_t bm;
	getBitmapBlock(blockSize, &bm, bitmapID + bitmap, fp);
	if(bm.bitmap[(blockID - (bitmapID * blockSize)) / 8] & (1 << ((blockID - (bitmapID * blockSize)) % 8))) {
		bm.bitmap[(blockID - (bitmapID * blockSize)) / 8] ^= (1 << ((blockID - (bitmapID * blockSize)) % 8));
		writeBitmap(&bm, fp, blockSize, bitmap + bitmapID);
	}
}
