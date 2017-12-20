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
                //check if no file with this name is on the disk
                for (int i = 0; i < sb.inodeBitmapSize; i++) {
                    bitmap_t bm;
                    getBitmapBlock(sb.blockSize, &bm, sb.inodeBitmapOffset + i, diskFile);
                    for (int j = 0; j < sb.blockSize; j++) {
                        for (int k = 0; k < 8; k++) {
                            if(bm.bitmap[j] & (1 << k)) {
                                inode_t inode;
                                getInodeBlock(sb.blockSize, sb.inodeList + i * sb.blockSize + j * 8 + k, sb.inodeList, &inode, diskFile);
                                if(strcmp(inode.name, argv[1]) == 0) {
                                    printf("A file with this name is already on the disk !\n");
                                    return 1;
                                }
                            }
                        }
                    }
                }

                //We can add the file
                int iNodeID = 0;
                allocBlock(sb.inodeBitmapSize, sb.inodeBitmapOffset, sb.blockSize, diskFile, &iNodeID);
                sb.inodesCount++;

                inode_t file;
                memset(&file, 0, sizeof(file));
                strcpy(file.name, argv[1]);

                char buffer[sb.blockSize];
                int bytesRead = 0;
                int blockID = 0;
                int iblockID = 0;
                int newIndirect = 1;
                int iOffset = 0;
                //Read the file block by block
                while ((bytesRead = fread(buffer, 1, sizeof(buffer), localFile)) > 0) {
                    file.exactSize += bytesRead;
                    int dataID = -1;
                    allocBlock(sb.dataBitmapSize, sb.dataBitmapOffset, sb.blockSize, diskFile, &dataID);
                    sb.dataBlockUsed++;
                    if(blockID < DIRECT_BLOCK_COUNT) {
                        bitmap_t data = {.bitmap = (uint8_t*)buffer};
                        writeBitmap(&data, diskFile, sb.blockSize, sb.inodeList + (sb.inodeMax * INODE_SIZE / sb.blockSize) + dataID);
                        file.blocks[blockID] = dataID;
                        blockID++;
                    } else {
                        if(newIndirect) {
                            newIndirect = 0;
                            file.indirectBlocks[iblockID] = dataID;
                            allocBlock(sb.dataBitmapSize, sb.dataBitmapOffset, sb.blockSize, diskFile, &dataID);
                            sb.dataBlockUsed++;
                        }
                        bitmap_t data = {.bitmap = (uint8_t*)buffer};
                        writeBitmap(&data, diskFile, sb.blockSize, sb.inodeList + (sb.inodeMax * INODE_SIZE / sb.blockSize) + dataID);
                        writeAddress(sb.inodeList + (sb.inodeMax * INODE_SIZE / sb.blockSize) + file.indirectBlocks[iblockID], iOffset, sb.blockSize, dataID, diskFile);
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
                writeSuperblock(&sb, diskFile, sb.blockSize);
                writeInode(&file, diskFile, sb.blockSize, sb.inodeList + (iNodeID * INODE_SIZE * 1.0 / sb.blockSize * 1.0));
            }
        }
    }
    return 0;
}
