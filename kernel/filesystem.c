#include "filesystem.h"

static superblock_t superblock;
static bitmap_t inodeBitmap;
//static bitmap_t dataBitmap;
static file_descriptor_t fileDescriptorTable[FILE_DESCRIPTORS_COUNT];

inline static void getInode(int inodeNum, inode_t* inode) {
	read_sector((superblock.inodeList * (superblock.blockSize / SECTOR_SIZE)) + inodeNum, inode);
}

inline static bool isFDValid(int fd) {
	return !(fd < 0 || fd > FILE_DESCRIPTORS_COUNT || fileDescriptorTable[fd].state == CLOSED);
}

static int getInodeFromFilename(char *filename, inode_t* inode) {
	for(uint i = 0 ; i <= superblock.inodeMax / 8 ; i++) {
		for(int j = 0 ; j < 8 ; j++) {
			if((inodeBitmap.bitmap[i] & (1 << j)) == 0)
				continue;
			getInode(i * 8 + j, inode);
			if(strcmp(inode->name, filename) == 0)
				return i * 8 + j;
		}
	}

	return -1;
}

static int iterateToNextInode(file_iterator_t *it) {
	for(uint i = it->currentInodeIndex + 1 ; i < superblock.inodeMax ; i++) {
		uint byteIndex = i / 8;
		uint bitIndex = i % 8;
		if((inodeBitmap.bitmap[byteIndex] & (1 << bitIndex)) != 0)
			return i;
	}
	return -1;
}

static void read_block(uint offset, uint8_t* dest) {
	uint8_t read[SECTOR_SIZE];
	int sectorByBlock = superblock.blockSize / SECTOR_SIZE;
	int sectorOffset = offset * sectorByBlock;
	for(int i = 0 ; i < sectorByBlock ; i++) {
		read_sector(sectorOffset + i, read);
		memcpy((void*) dest + (i * SECTOR_SIZE), read, SECTOR_SIZE);
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

	//for(int i = 0 ; i < 1024 / 8 ; i++)
	//	printf("%d\t", inodeBitmap.bitmap[i]);

	//uint8_t data_bytes[superblock.dataBitmapSize * superblock.blockSize];
	//for(int i = 0 ; i < superblock.dataBitmapSize ; i++) {
	//	read_block(superblock.dataBitmapOffset + i, &(data_bytes[i * superblock.blockSize]));
	//}
	//dataBitmap.bitmap = data_bytes;

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
	stat->size = inode.size;
	return 0;
}

bool file_exists(char *filename) {
	inode_t inode;
	return getInodeFromFilename(filename, &inode) == -1 ? false : true;
}

int file_open(char *filename) {
	inode_t inode;
	if(getInodeFromFilename(filename, &inode) == -1)
		return -1;

	for(int i = 0 ; i < FILE_DESCRIPTORS_COUNT ; i++) {
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
	if(!isFDValid(fd) | (count <= 0))
		return -1;

	file_descriptor_t* descriptor = &(fileDescriptorTable[fd]);

	if(descriptor->currentByte >= descriptor->inode.size)
		return 0;
	if(descriptor->currentByte + count > descriptor->inode.size)
		count -= (descriptor->currentByte + count) - descriptor->inode.size;

	uint startBlock = descriptor->currentByte / superblock.blockSize;
	uint endBlock = (descriptor->currentByte + count) / superblock.blockSize;

	int byteCount = 0;

	for(uint i = startBlock ; i <= endBlock ; i++) {
		uint8_t block[superblock.blockSize];
		if(i < DIRECT_BLOCK_COUNT)
			read_block(superblock.dataBlockOffset + descriptor->inode.blocks[i], block);
		else if(i < DIRECT_BLOCK_COUNT + (INDIRECT_BLOCK_COUNT * (superblock.blockSize / sizeof(indirectBlock_t)))) {
			indirectBlock_t indirectBlock;
			int indirectBlockIndex = (i - DIRECT_BLOCK_COUNT) / (superblock.blockSize / sizeof(indirectBlock_t));
			read_block(superblock.dataBlockOffset + descriptor->inode.indirectBlocks[indirectBlockIndex], (uint8_t*) &indirectBlock);

			int positionInBlock = (i - DIRECT_BLOCK_COUNT) % (superblock.blockSize / sizeof(indirectBlock_t));
			read_block(superblock.dataBlockOffset + indirectBlock.addresses[positionInBlock], block);
		}
		else
			return -1;

		if(i == startBlock && (descriptor->currentByte % superblock.blockSize) + count < superblock.blockSize) {
			memcpy(buf + byteCount, &(block[descriptor->currentByte % superblock.blockSize]), count);
			byteCount += count;
			descriptor->currentByte += count;
		}
		else if(i == startBlock) {
			memcpy(buf + byteCount, 
				&(block[descriptor->currentByte % superblock.blockSize]), 
				superblock.blockSize - (descriptor->currentByte % superblock.blockSize));
			byteCount += superblock.blockSize - (descriptor->currentByte % superblock.blockSize);
			descriptor->currentByte += superblock.blockSize - (descriptor->currentByte % superblock.blockSize);
		}
		else if(i == endBlock) {
			memcpy(buf + byteCount, block, count - byteCount);
			byteCount += count;
			descriptor->currentByte += count;
		}
		else {
			memcpy(buf + byteCount, block, superblock.blockSize * SECTOR_SIZE);
			byteCount += superblock.blockSize * SECTOR_SIZE;
			descriptor->currentByte += superblock.blockSize * SECTOR_SIZE;
		}
	}

	return byteCount;
}

int file_seek(int fd, uint offset) {
	if(!isFDValid(fd) || offset >= fileDescriptorTable[fd].inode.size)
		return -1;
	fileDescriptorTable[fd].currentByte = offset;
	return fileDescriptorTable[fd].currentByte;
}

void file_close(int fd) {
	if(!isFDValid(fd))
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

	if(it->state == CREATED && (inodeBitmap.bitmap[0] & 0x1) != 0) {
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
