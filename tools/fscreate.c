#include "fsutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char const *argv[]) {
    if(argc < 6) {
        printf("Usage: %s <label> <block_size> <out_file> <max_files> <max_blocks>\n", argv[0]);
        return 1;
    }
    const char* label = argv[1];
    int bs = atoi(argv[2]);
    const char* outFile = argv[3];
    int maxFiles = atoi(argv[4]);
    int maxBlocks = atoi(argv[5]);

    if(bs % 512 != 0) {
        printf("Error: Block size must be a multiple of 512\n");
        return 1;
    }

    uint32_t inodeBMSize = (uint32_t) ceil((maxFiles * 1.0) / (bs * 1.0));
    int inodesPerBlocks = bs / INODE_SIZE;
    int remainingDataBMDataBlock = maxBlocks - 1 - (inodeBMSize * bs / inodesPerBlocks) - inodeBMSize;
    if(remainingDataBMDataBlock < 1) {
        printf("Your disk size is too small with that block size, please specify a bigger disk or smaller blocks\n");
        return 1;
    }
    uint32_t dataBMSize = (uint32_t) ceil((remainingDataBMDataBlock * 1.0) / (bs * 1.0));
    uint32_t dataBlockSize = remainingDataBMDataBlock - dataBMSize;

    printf("Number of inodes bitmaps: %d\tNumber of data bitmaps: %d\tNumber of data blocks: %d\n", inodeBMSize, dataBMSize, dataBlockSize);

    superblock_t super = {
        .magic = SUPERBLOCK_MAGIC,
		.version = FS_VERSION,
		.blockSize = bs,
		.inodeCount = maxFiles,
		.inodeBitmapSize = inodeBMSize,
		.inodeBitmapOffset = INODE_BITMAP_OFFSET,
		.dataBitmapSize = dataBMSize,
		.dataBitmapOffset = INODE_BITMAP_OFFSET + inodeBMSize,
		.inodeList = INODE_BITMAP_OFFSET + inodeBMSize + dataBMSize,
    };

    strcpy(&(super.label[0]), label);

    FILE *diskFile = fopen(outFile, "wb");
    if (diskFile != NULL) {
        writeSuperblock(&super, diskFile, bs);
        for (uint32_t i = 0; i < inodeBMSize + dataBMSize; i++) {
            bitmap_t bm;
            bm.bitmap = (uint8_t*) calloc(bs, sizeof(uint8_t));
            writeBitmap(&bm, diskFile, bs, i + INODE_BITMAP_OFFSET);
            free(bm.bitmap);
        }
        //fill the disk with zeros
        for (uint32_t i = 0; i < ((maxFiles / inodesPerBlocks) + dataBlockSize) * bs; i++) {
        	fputc(0, diskFile);
        }
        fclose (diskFile);
    } else {
        printf("Error while creating the disk file\n");
        return 1;
    }
    return 0;
}
