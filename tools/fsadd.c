#include "fsutil.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    if(argc < 3) {
        printf("Usage: %s <file> <disk_image>\n", argv[0]);
    }
    FILE* diskFile = fopen(argv[2], "rb+");
    FILE* localFile = fopen(argv[1], "rb");
    if(diskFile != NULL) {
        if(localFile != NULL) {
            superblock_t sb;
            readSuperblock(diskFile, &sb);
            if(sb.magic == SUPERBLOCK_MAGIC) {
                int iNodeID = 0;
                allocBlock(sb.inodeBitmapSize, sb.inodeBitmapOffset, sb.blockSize, diskFile, &iNodeID);

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
                int newIndirect = 1;
                int iOffset = 0;
                while ((bytesRead = fread(buffer, 1, sizeof(buffer), localFile)) > 0) {
                    file.size += 1;
                    int dataID = -1;
                    allocBlock(sb.dataBitmapSize, sb.dataBitmapOffset, sb.blockSize, diskFile, &dataID);
                    if(blockID < DIRECT_BLOCK_COUNT) {
                        bitmap_t data = {.bitmap = (uint8_t*)buffer};
                        writeBitmap(&data, diskFile, sb.blockSize, sb.inodeList + (sb.inodeCount * INODE_SIZE / sb.blockSize) + dataID);
                        file.blocks[blockID] = dataID;
                        blockID++;
                    } else {
                        if(newIndirect) {
                            newIndirect = 0;
                            file.indirectBlocks[iblockID] = dataID;
                            allocBlock(sb.dataBitmapSize, sb.dataBitmapOffset, sb.blockSize, diskFile, &dataID);
                        }
                        bitmap_t data = {.bitmap = (uint8_t*)buffer};
                        writeBitmap(&data, diskFile, sb.blockSize, sb.inodeList + (sb.inodeCount * INODE_SIZE / sb.blockSize) + dataID);
                        writeAddress(sb.inodeList + (sb.inodeCount * INODE_SIZE / sb.blockSize) + file.indirectBlocks[iblockID], iOffset, sb.blockSize, dataID, diskFile);
                        iOffset += 4;
                        if(iOffset == sb.blockSize) {
                            newIndirect = 1;
                            iOffset = 0;
                            iblockID++;
                            if(iblockID > 3) {
                                printf("File is too big\n");
                                return 1;
                            }
                        }
                    }
                }
                writeInode(&file, diskFile, sb.blockSize, sb.inodeList + (iNodeID * INODE_SIZE * 1.0 / sb.blockSize * 1.0));
            }
        }
    }
    return 0;
}
