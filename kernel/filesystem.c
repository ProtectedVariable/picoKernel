#include "filesystem.h"

static superblock_t superblock;
static bitmap_t inodeBitmap;
static file_descriptor_t fileDescriptorTable[FILE_DESCRIPTORS_COUNT];

/**
 * Get an inode from the filesystem given an inode index
 * @param  inodeNum   	The index of an inode to get
 * @param  inode 		The structure to which the inode will be printed
 * @return           	-1 in case of failure, 0 otherwise
 */
inline static void getInode(int inodeNum, inode_t* inode) {
	read_sector((superblock.inodeList * superblock.blockSize / SECTOR_SIZE) + inodeNum, inode);
}

/**
 * Test if a file descriptor is in the correct range or closed
 * @param  fd   The index of the file descriptor to test
 * @return      true if the file descriptor is wrong, false otherwise
 */
inline static bool isFDWrong(int fd) {
	return fd < 0 || fd > FILE_DESCRIPTORS_COUNT || fileDescriptorTable[fd].state == CLOSED;
}

/**
 * Search for the index of the inode representing a certain filename
 * @param  filename   	The name of the file to look for
 * @param  inode 		The structure of an inode to be loaded when found
 * @return           	-1 if the file is not found, the number of the inode otherwise
 */
static int getInodeFromFilename(char *filename, inode_t* inode) {
	for(uint i = 0 ; i < superblock.inodeMax / 8; i++) {
		for(int j = 0 ; j < 8 ; j++) {
			if((inodeBitmap.bitmap[i] & (1 << (7-j))) == 0)
				continue;
			getInode(i * 8 + j, inode);
			if(strcmp(inode->name, filename) == 0) {
				return i * 8 + j;
			}
		}
	}
	return -1;
}

/**
 * Get the inode number of the next file represented by an iterator
 * @param  it   The iterator to use
 * @return      -1 if there are no more file, or the index of the inode
 */
static int iterateToNextInode(file_iterator_t *it) {
	for(uint i = it->currentInodeIndex + 1 ; i < superblock.inodeMax ; i++) {
		uint byteIndex = i / 8;
		uint bitIndex = i % 8;
		if((inodeBitmap.bitmap[byteIndex] & (1 << bitIndex)) != 0)
			return i;
	}
	return -1;
}

/**
 * Get the bytes of a block from the filesystem sector by sector
 * @param  offset   The index of the block to retrieve
 * @param  dest 	The buffer to receive the bytes
 */
static void read_block(uint offset, uint8_t* dest) {
	int sectorByBlock = superblock.blockSize / SECTOR_SIZE;
	int sectorOffset = offset * sectorByBlock;
	for(int i = 0; i < sectorByBlock; i++) {
		read_sector(sectorOffset + i, &(dest[i * SECTOR_SIZE]));
	}
}

void filesystem_init() {
	read_sector(0, &superblock);
	if(superblock.magic != SUPERBLOCK_MAGIC) {
		setColor(RED);
		printf("WRONG FILESYSTEM LOADED : picoFS expected\n");
		printf("KERNEL PANIC.");
		halt();
	}

	uint8_t bitmap_bytes[superblock.inodeBitmapSize * superblock.blockSize];
	for(int i = 0 ; i < superblock.inodeBitmapSize ; i++) {
		read_block(superblock.inodeBitmapOffset + i, &(bitmap_bytes[i * superblock.blockSize]));
	}
	inodeBitmap.bitmap = bitmap_bytes;

	for(int i = 0 ; i < FILE_DESCRIPTORS_COUNT ; i++) {
		fileDescriptorTable[i].currentByte = 0;
		fileDescriptorTable[i].state = CLOSED;
	}

	printf("%s v.%d Initialized\n", superblock.label, superblock.version);
}

int file_stat(char *filename, stat_t *stat) {
	inode_t inode;
	if(getInodeFromFilename(filename, &inode) == -1)
		return -1;
	stat->name = filename;
	stat->size = inode.exactSize;
	return 0;
}

bool file_exists(char *filename) {
	inode_t inode;
	return getInodeFromFilename(filename, &inode) == -1 ? false : true;
}

int file_open(char *filename) {
	inode_t inode;
	if(getInodeFromFilename(filename, &inode) == -1) {
		return -1;
	}

	for(int i = 0 ; i < FILE_DESCRIPTORS_COUNT; i++) {
		if(fileDescriptorTable[i].state == CLOSED) {
			fileDescriptorTable[i].state = OPENED;
			fileDescriptorTable[i].currentByte = 0;
			memcpy(&(fileDescriptorTable[i].inode), &inode, SECTOR_SIZE);
			return i;
		}
	}

	return -1;
}

int file_read(int fd, void *buf, uint count) {
	if(isFDWrong(fd) || count <= 0)
		return -1;

	file_descriptor_t* descriptor = &(fileDescriptorTable[fd]);

	if(descriptor->currentByte >= descriptor->inode.exactSize)
		return 0;
	if(descriptor->currentByte + count > descriptor->inode.exactSize)
		count -= (descriptor->currentByte + count) - descriptor->inode.exactSize;

	uint startBlock = descriptor->currentByte / superblock.blockSize;
	uint endBlock = (descriptor->currentByte + count) / superblock.blockSize;
	int byteCount = 0;
	uint8_t block[superblock.blockSize];
	for(uint i = startBlock; i <= endBlock; i++) {
		if(i < DIRECT_BLOCK_COUNT) {
			read_block(superblock.dataBlockOffset + descriptor->inode.blocks[i], block);
		} else if(i < DIRECT_BLOCK_COUNT + (INDIRECT_BLOCK_COUNT * (superblock.blockSize / sizeof(uint32_t)))) {
			indirectBlock_t indirectBlock;
			uint32_t addr[superblock.blockSize / sizeof(uint32_t)];
			indirectBlock.addresses = addr;

			int indirectBlockIndex = (i - DIRECT_BLOCK_COUNT) / (superblock.blockSize / sizeof(uint32_t));
			read_block(superblock.dataBlockOffset + descriptor->inode.indirectBlocks[indirectBlockIndex], (uint8_t*) indirectBlock.addresses);
			int positionInBlock = (i - DIRECT_BLOCK_COUNT) % (superblock.blockSize / sizeof(indirectBlock_t));
			read_block(superblock.dataBlockOffset + indirectBlock.addresses[positionInBlock], block);
		} else {
			return -1;
		}


		if(i == startBlock && i == endBlock) {
			memcpy(buf + byteCount, &(block[descriptor->currentByte % superblock.blockSize]), count);
			byteCount += count;
			descriptor->currentByte += count;
		}
		else if(i == startBlock) {
			memcpy(buf, &(block[descriptor->currentByte % superblock.blockSize]), superblock.blockSize - (descriptor->currentByte % superblock.blockSize));
			byteCount += superblock.blockSize - (descriptor->currentByte % superblock.blockSize);
			descriptor->currentByte += superblock.blockSize - (descriptor->currentByte % superblock.blockSize);
		}
		else if(i == endBlock) {
			memcpy(buf + byteCount, block, count - byteCount);
			descriptor->currentByte += count - byteCount;
			byteCount += count - byteCount;
		}
		else {
			memcpy(buf + byteCount, block, superblock.blockSize);
			byteCount += superblock.blockSize;
			descriptor->currentByte += superblock.blockSize;
		}
	}

	return byteCount;
}

int file_seek(int fd, uint offset) {
	if(isFDWrong(fd) || offset >= fileDescriptorTable[fd].inode.exactSize)
		return -1;
	fileDescriptorTable[fd].currentByte = offset;
	return fileDescriptorTable[fd].currentByte;
}

void file_close(int fd) {
	if(isFDWrong(fd))
		return;
	fileDescriptorTable[fd].state = CLOSED;
}

file_iterator_t file_iterator() {
	file_iterator_t result;
	result.currentInodeIndex = 0;
	result.nextInode = -1;
	result.state = CREATED;

	return result;
}

bool file_has_next(file_iterator_t *it) {
	if(it->state == FINISHED)
		return false;

	if(it->state == CREATED && ((inodeBitmap.bitmap[0] & (1 << 7)) != 0)) {
		it->nextInode = 0;
		return true;
	}

	int nextInodeIndex = iterateToNextInode(it);
	if(nextInodeIndex != -1) {
		it->nextInode = nextInodeIndex;
		return true;
	}

	it->state = FINISHED;
	return false;
}

void file_next(char *filename, file_iterator_t *it) {
	if(it->state == FINISHED)
		return;
	else if(it->nextInode < 0) {
		int nextInode = iterateToNextInode(it);
		if(nextInode == -1) {
			it->state = FINISHED;
			return;
		}

		it->nextInode = nextInode;
	}

	if(it->state == CREATED)
		it->state = ITERATING;

	inode_t inode;
	getInode(it->nextInode, &inode);
	it->currentInodeIndex = it->nextInode;
	it->nextInode = -1;
	memcpy(filename, &(inode.name), FILENAME_MAXSIZE);
}
