#include "filesystem.h"

void filesystem_init() {
	uint8_t superblock[SUPERBLOCK_SIZE];
	read_sector(0, superblock);
	for(int i = 0 ; i < SUPERBLOCK_SIZE ; i++)
		printf("%x\n", superblock[i]);
}

//int file_stat(char *filename, stat_t *stat);
// bool file_exists(char *filename);
// int file_open(char *filename);
// int file_read(int fd, void *buf, uint count);
// int file_seek(int fd, uint offset);
// void file_close(int fd);
// file_iterator_t file_iterator();
// bool file_has_next(file_iterator_t *it);
// void file_next(char *filename, file_iterator_t *it);
