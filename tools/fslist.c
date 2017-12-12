#include "fsutil.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    if(argc < 2) {
        printf("Usage: %s <disk_file>\n", argv[0]);
    }
    FILE* diskFile = fopen(argv[1], "rb+");
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
                            printf("%s: %d bytes (%d bytes on the disk)\n", inode.name, inode.exactSize, inode.size * sb.blockSize);
                        }
                    }
                }
            }
        }
    }
    return 0;
}
