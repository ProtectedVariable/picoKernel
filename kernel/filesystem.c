#include "filesystem.h"

static superblock_t superblock;
static bitmap_t inodeBitmap;
static bitmap_t dataBitmap;
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

	uint8_t data_bytes[superblock.dataBitmapSize * superblock.blockSize];
	for(int i = 0 ; i < superblock.dataBitmapSize ; i++) {
		read_block(superblock.dataBitmapOffset + i, &(data_bytes[i * superblock.blockSize]));
	}
	dataBitmap.bitmap = data_bytes;

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
			memcpy(&(fileDescriptorTable[i].inode), &inode, sizeof(inode_t));
			return i;
		}
	}

	return -1;
}

// int file_read(int fd, void *buf, uint count);

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

// file_iterator_t file_iterator();
// bool file_has_next(file_iterator_t *it);
// void file_next(char *filename, file_iterator_t *it);
