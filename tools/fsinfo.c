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
            printf("---------------------picoFS disk---------------------\n");
            printf("Label: %s\n", sb.label);
            printf("Version: %d\n", sb.version);
            printf("Blocks of %d bytes each\n", sb.blockSize);
            printf("Maximum amount of files on the disk: %d\n", sb.inodeMax);
            printf("Current amount of files on the disk: %d (%f %%)\n", sb.inodesCount, (float)sb.inodesCount / (float)sb.inodeMax);
            printf("Total Space: %u bytes\n", sb.dataBlockCount * sb.blockSize);
            printf("Used Space: %u bytes (%f %%)\n", sb.dataBlockUsed * sb.blockSize, (float)sb.dataBlockUsed / (float)sb.dataBlockCount);
            printf("-----------------------------------------------------\n");
        }
    }
    return 0;
}
