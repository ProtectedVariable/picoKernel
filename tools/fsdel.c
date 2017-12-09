#include "fsutil.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    if(argc < 3) {
        printf("Usage: %s <file> <disk_image>\n", argv[0]);
    }
    FILE* diskFile = fopen(argv[2], "rb+");
    if(diskFile != NULL) {
        superblock_t sb;
        readSuperblock(diskFile, &sb);
        if(sb.magic == SUPERBLOCK_MAGIC) {
            for (int i = 0; i < sb.inodeBitmapSize; i++) {
                bitmap_t bm;
                getBitmapBlock(sb.blockSize, &bm, sb.inodeBitmapOffset + i, diskFile);
                for (int j = 0; j < sb.blockSize; j++) {
                    for (int k = 0; k < 8; k++) {
                        if(bm.bitmap[j] & (1 << k)) {
                            inode_t inode;
                            getInodeBlock(sb.blockSize, sb.inodeList + i * sb.blockSize + j * 8 + k, sb.inodeList, &inode, diskFile);
                            if(strcmp(inode.name, argv[1]) == 0) {
                                printf("Found File\n");
                                /*
                                int iblockID = 0;
                                int newIndirect = 1;
                                int indirectBlockCount = 0;*/

                                for (int b = 0; b < inode.size; b++) {
                                    if(b < DIRECT_BLOCK_COUNT) {
                                        freeBlock(sb.dataBitmapOffset, sb.blockSize, inode.blocks[b], diskFile);
                                    } else {
                                        /*
                                        if(newIndirect) {
                                            iblockID = inode.indirectBlocks[indirectBlockCount];
                                            newIndirect = 0;
                                            freeBlock(sb.dataBitmapOffset, sb.blockSize, iblockID, diskFile);
                                        }
                                        freeBlock(int dataBitmap, int blockSize, int blockID, FILE *fp)
                                        */
                                    }
                                }
                                freeBlock(sb.inodeBitmapOffset, sb.blockSize, i * sb.blockSize + j * 8 + k, diskFile);
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}
