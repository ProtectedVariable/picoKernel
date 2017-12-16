#ifndef PROTECC_ATTACC_INCLUDECC
#define PROTECC_ATTACC_INCLUDECC

#include "../common/types.h"
#include "x86.h"
#include "ide.h"
#include "display.h"

#define SECTOR_SIZE 512
#define INODE_SIZE 512
#define SUPERBLOCK_SIZE 512
#define FILENAME_MAXSIZE 40
#define DIRECT_BLOCK_COUNT 113
#define INDIRECT_BLOCK_COUNT 4
#define SUPERBLOCK_MAGIC 0xCA11AB1E
#define FILE_DESCRIPTORS_COUNT 256

typedef struct superblock_st {
	uint32_t magic;
	uint32_t version;
	uint16_t blockSize;
	uint32_t inodeMax;
	uint8_t inodeBitmapSize;
	uint16_t inodeBitmapOffset;
	uint8_t dataBitmapSize;
	uint16_t dataBitmapOffset;
	uint16_t inodeList;
	uint32_t inodesCount;
	uint32_t dataBlockCount;
	uint32_t dataBlockUsed;
	char label[478];
}__attribute__((packed)) superblock_t;

typedef struct inode_st {
	char name[FILENAME_MAXSIZE];
	uint32_t blocks[DIRECT_BLOCK_COUNT];
	uint32_t indirectBlocks[INDIRECT_BLOCK_COUNT];
	uint32_t size;
}__attribute__((packed)) inode_t;

typedef struct indirectBlock_st {
	uint32_t* addresses;
}__attribute__((packed)) indirectBlock_t;

typedef struct bitmap_st {
	uint8_t* bitmap;
}__attribute__((packed)) bitmap_t;

typedef struct stat_st {
	char* name;
	uint size;
} stat_t;

typedef struct file_iterator_st {
	char* name;
	uint size;
} file_iterator_t;

typedef enum file_state_st {
	OPENED,
	CLOSED
} file_stat_t;

typedef struct file_descriptor_st {
	uint32_t currentByte;
	file_stat_t state;
	inode_t inode;
} file_descriptor_t;

extern void filesystem_init();
extern int file_stat(char *filename, stat_t *stat);
extern bool file_exists(char *filename);

extern int file_open(char *filename);
// Ouvre un fichier et renvoie un descripteur de fichier pour y accéder ou -1 en cas d'échec.

extern int file_read(int fd, void *buf, uint count);
// Essaie de lire count bytes depuis le fichier référencé par
// fd et les place dans le buffer buf.
// Renvoie le nombre de bytes lus, ou 0 en cas de fin de fichier, ou -1 en cas d’erreur.

extern int file_seek(int fd, uint offset);
// Positionne la position pointeur du fichier ouvert (référencé par le descripteur
// fd) à offset par rapport au début du fichier.
// Renvoie la nouvelle position ou -1 en cas d’échec.

extern void file_close(int fd);
// Ferme le fichier référencé par le descripteur fd

extern file_iterator_t file_iterator();
// Créé un itérateur permettant d'itérer sur les fichiers du système de fichiers.

extern bool file_has_next(file_iterator_t *it);
// Renvoie true si il y a encore un fichier sur lequel itérer.
//
// extern void file_next(char *filename, file_iterator_t *it);
// Copie dans filename le nom du prochain fichier pointé par l’itérateur

#endif
