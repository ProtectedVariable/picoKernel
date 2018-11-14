#ifndef PROTECC_ATTACC_INCLUDECC
#define PROTECC_ATTACC_INCLUDECC

#include "../common/types.h"
#include "../common/picofs.h"
#include "x86.h"
#include "ide.h"
#include "display.h"
#include "fsdata.h"

#define FILE_DESCRIPTORS_COUNT 256

/**
 * Enumeration representing the state of a file in a file descriptor
 */
typedef enum file_state_st {
	OPENED,
	CLOSED
} file_state_t;

/**
 * Structure representing a file descriptor
 */
typedef struct file_descriptor_st {
	uint32_t currentByte;
	file_state_t state;
	inode_t inode;
} file_descriptor_t;

/**
 * Initializes the filesystem by loading the superblock and bitmaps in memory, and initalizing the file descriptor table
 */
extern void filesystem_init();

/**
 * Returns a structure containing the name and size of a file
 * @param  filename   	The file to look for
 * @param  stat 		The structure to which the info about the file will be written
 * @return           	-1 in case of failure, 0 otherwise
 */
extern int file_stat(char *filename, stat_t *stat);

/**
 * Test the existence of a given file in the filesystem
 * @param  filename   	The file to look for
 * @return           	1 if the file exists, 0 otherwise
 */
extern bool file_exists(char *filename);

/**
 * Opens a file to seeking and reading operations, initializes a file descriptor
 * @param  filename   	The file to open
 * @return           	-1 in case of failure, the index of the file descriptor in the table otherwise
 */
extern int file_open(char *filename);

/**
 * Reads bytes from the file designated by the file descriptor and put them in a buffer
 * @param  fd   	The file descriptor's index representing the file to read from
 * @param  buf 		A buffer of at least the length of count to place the read bytes in.
 * @param  count 	The target number of bytes to read
 * @return          -1 in case of failure, 0 if no bytes could be read, the number of read bytes otherwise
 */
extern int file_read(int fd, void *buf, uint count);

/**
 * Positions the cursor for a certain file on a given byte.
 * @param  fd   	The file descriptor's index representing the file to use
 * @param  offset 	The number of bytes after which the cursor will be positionned
 * @return          -1 in case of failure, the new position of the cursor otherwise
 */
extern int file_seek(int fd, uint offset);

/**
 * Closes the file represented by a given file descriptor if its open, do nothing otherwise
 * @param  fd   The file descriptor to be closed
 */
extern void file_close(int fd);

/**
 * Returns a new file iterator
 * @Returns		a newly initialized file iterator to crawl through the files in the filesystem
 */
extern file_iterator_t file_iterator();

/**
 * Returns true if there are still files to iterate to, false otherwise
 * @param  it   The iterator to use
 * @return      true if there is still a file to call for, false otherwise
 */
extern bool file_has_next(file_iterator_t *it);

/**
 * Looks for the next file represented by the given iterator, and puts its filename into the filename string
 * @param  filename   	A string that will contain the next file's filename
 * @param  it 			The iterator to use
 */
extern void file_next(char *filename, file_iterator_t *it);

#endif
