#ifndef PROTECC_ATTACC_INCLUDECC
#define PROTECC_ATTACC_INCLUDECC

#include "../common/types.h"
#include "x86.h"
#include "ide.h"
#include "display.h"
#include "fsdata.h"

#define FILE_DESCRIPTORS_COUNT 256

typedef enum file_state_st {
	OPENED,
	CLOSED
} file_state_t;

typedef enum iterator_state_st {
	CREATED,
	ITERATING,
	FINISHED
} iterator_state_t;

typedef struct stat_st {
	char* name;
	uint size;
} stat_t;

typedef struct file_iterator_st {
	uint currentInodeIndex;
	int nextInode;
	iterator_state_t state;
} file_iterator_t;

typedef struct file_descriptor_st {
	uint32_t currentByte;
	file_state_t state;
	inode_t inode;
} file_descriptor_t;

extern void filesystem_init();
extern int file_stat(char *filename, stat_t *stat);
extern bool file_exists(char *filename);
extern int file_open(char *filename);
extern int file_read(int fd, void *buf, uint count);
extern int file_seek(int fd, uint offset);
extern void file_close(int fd);
extern file_iterator_t file_iterator();
extern bool file_has_next(file_iterator_t *it);
extern void file_next(char *filename, file_iterator_t *it);

#endif
