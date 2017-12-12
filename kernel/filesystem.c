#include "filesystem.h"

static superblock_t superblock;

void filesystem_init() {
	read_sector(0, &superblock);
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
