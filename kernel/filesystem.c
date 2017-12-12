#include "filesystem.h"

static superblock_t superblock;
static bitmap_t inodeBitmap;
// static bitmap_t dataBitmap;

void read_block(uint offset, void* dest) {
	uint8_t read[SECTOR_SIZE];
	int sectorByBlock = superblock.blockSize / SECTOR_SIZE;
	int sectorOffset = offset * sectorByBlock;
	for(int i = 0 ; i < sectorByBlock ; i++) {
		read_sector(sectorOffset + i, read);
		memcpy(dest + (i * SECTOR_SIZE), read, SECTOR_SIZE);
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

	uint8_t* bitmap_bytes[superblock.inodeBitmapSize * superblock.blockSize];
	for(int i = 0 ; i < superblock.inodeBitmapSize ; i++) {
		read_block(superblock.inodeBitmapOffset + i, bitmap_bytes[i * superblock.blockSize]);
	}
	memcpy(&inodeBitmap, bitmap_bytes, superblock.inodeBitmapSize * superblock.blockSize);


	printf("%s v.%d Initialized\n", superblock.label, superblock.version);
	printf("LOL : %d\n", inodeBitmap.bitmap[0]);
}

int file_stat(char *filename, stat_t *stat) {
	printf("ISSOU\n");
}

// bool file_exists(char *filename);
// int file_open(char *filename);
// int file_read(int fd, void *buf, uint count);
// int file_seek(int fd, uint offset);
// void file_close(int fd);
// file_iterator_t file_iterator();
// bool file_has_next(file_iterator_t *it);
// void file_next(char *filename, file_iterator_t *it);
