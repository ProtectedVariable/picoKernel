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

                                int iblockID = 0;
                                int newIndirect = 1;
                                int indirectBlockCount = 0;
                                int indirectOffset = 0;

                                for (size_t b = 0; b < inode.size; b++) {
                                    if(b < DIRECT_BLOCK_COUNT) {
                                        freeBlock(sb.dataBitmapOffset, sb.blockSize, inode.blocks[b], diskFile);
                                        sb.dataBlockUsed--;
                                    } else {
                                        if(newIndirect) {
                                            newIndirect = 0;
                                            if(indirectBlockCount >= INDIRECT_BLOCK_COUNT) {
                                                printf("Disk format error\n");
                                                return 1;
                                            }
                                            iblockID = inode.indirectBlocks[indirectBlockCount];
                                            freeBlock(sb.dataBitmapOffset, sb.blockSize, iblockID, diskFile);
                                            sb.dataBlockUsed--;
                                        }
                                        freeBlock(sb.dataBitmapOffset, sb.blockSize, readAddress(sb.inodeList + (sb.inodeMax * INODE_SIZE / sb.blockSize) + iblockID, indirectOffset, sb.blockSize, diskFile), diskFile);
                                        sb.dataBlockUsed--;
                                        indirectOffset += sizeof(uint32_t);
                                        if(indirectOffset == sb.blockSize) {
                                            indirectOffset = 0;
                                            indirectBlockCount++;
                                            newIndirect = 1;
                                        }
                                    }
                                }
                                freeBlock(sb.inodeBitmapOffset, sb.blockSize, i * sb.blockSize + j * 8 + k, diskFile);
                                sb.inodesCount--;
                                writeSuperblock(&sb, diskFile, sb.blockSize);
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
