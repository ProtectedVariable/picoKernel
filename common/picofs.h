#ifndef _PICO_FS_H_
#define _PICO_FS_H_

#include "types.h"

/**
 * Enumeration representing the state of an iterator
 */
typedef enum iterator_state_st {
	CREATED,
	ITERATING,
	FINISHED
} iterator_state_t;

/**
 * Structure representing the informations on a file
 */
typedef struct stat_st {
	char* name;
	uint size;
} stat_t;

/**
 * Structure representing a file iterator
 */
typedef struct file_iterator_st {
	uint currentInodeIndex;
	int nextInode;
	iterator_state_t state;
} file_iterator_t;

#endif /* end of include guard: _PICO_FS_H_ */
