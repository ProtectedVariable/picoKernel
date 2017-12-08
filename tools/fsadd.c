#include "fsutil.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    if(argc < 3) {
        printf("Usage: fs_add <file> <disk_image>\n");
    }
    FILE* diskFile = fopen(argv[2], "rb+");
    FILE* localFile = fopen(argv[1], "rb");
    if(diskFile != NULL) {
        if(localFile != NULL) {
            superblock_t sb;
            readSuperblock(diskFile, &sb);
            if(sb.magic == SUPERBLOCK_MAGIC) {
                bitmap_t bm;
                int id = -1;
                int bitmapPosition = 0;
                for (size_t i = 0; i < sb.inodeBitmapSize; i++) {
                    getBitmapBlock(sb.blockSize, &bm, i + sb.inodeBitmapOffset, diskFile);
                    id = getFirstUnusedID(&bm, sb.blockSize);
                    if(id != -1) {
                        bitmapPosition = i + sb.inodeBitmapOffset;
                        break;
                    }
                }
                if(id == -1) {
                    printf("Disk Full !\n");
                }
                bm.bitmap[id / 8] |= (1 << (id % 8));
                writeBitmap(&bm, diskFile, sb.blockSize, bitmapPosition);

                inode_t file;
                memset(file.name, 0, FILENAME_MAXSIZE);
                strcpy(file.name, argv[1]);
                memset(file.blocks, 0, DIRECT_BLOCK_COUNT * sizeof(uint32_t));
                memset(file.indirectBlocks, 0, INDIRECT_BLOCK_COUNT * sizeof(uint32_t));
                file.size = 0;

                char buffer[sb.blockSize];
                int bytesRead = 0;
                int blockID = 0;
                int iblockID = 0;
                while ((bytesRead = fread(buffer, 1, sizeof(buffer), localFile)) > 0) {
                    file.size += 1;
                    int dataID = -1;
                    int dataBMPosition = 0;
                    for (int i = 0; i < sb.dataBitmapSize; i++) {
                        getBitmapBlock(sb.blockSize, &bm, i + sb.dataBitmapOffset, diskFile);
                        dataID = getFirstUnusedID(&bm, sb.blockSize);
                        if(dataID != -1) {
                            dataBMPosition = i + sb.dataBitmapOffset;
                            break;
                        }
                    }
                    if(dataID == -1) {
                        printf("Disk Full");
                    }
                    bm.bitmap[dataID / 8] |= (1 << (dataID % 8));
                    writeBitmap(&bm, diskFile, sb.blockSize, dataBMPosition);
                    if(blockID < DIRECT_BLOCK_COUNT) {
                        bitmap_t data = {.bitmap = (uint8_t*)buffer};
                        writeBitmap(&data, diskFile, sb.blockSize, sb.inodeList + (sb.inodeCount * INODE_SIZE / sb.blockSize) + dataID);
                        file.blocks[blockID] = dataID;
                        blockID++;
                    } else {
                        //TODO Indirect Blocks
                    }
                }
                writeInode(&file, diskFile, sb.blockSize, id + sb.inodeList);
            }
        }
    }
    return 0;
}
