#ifndef MINHELPER_H
#define MINHELPER_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#define PARTTABLE_LOC 0x1BE
#define PART_TYPE 0x81
#define BYTE_510 0x55
#define BYTE_511 0xAA
#define MINIX_MAGIC 0x4D5A
#define MINIX_MAGIC_R 0x5A4D
#define INODE_SIZE 64
#define DIR_ENTRY_SIZE 64
#define OFFSET 1024
#define ROOT_DIR 1
typedef struct 
{ /* Minix Version 3 Superblock
* this structure found in fs/super.h
* * in minix 3.1.1
* */
/* on disk. These fields and orientation are non–negotiable */
	uint32_t ninodes; /* number of inodes in this filesystem */
	uint16_t pad1; /* make things line up properly */
	int16_t i_blocks; /* # of blocks used by inode bit map */
	int16_t z_locks; /* # of blocks used by zone bit map */
	uint16_t firstdata; /* number of first data zone */
	int16_t log_zone_size; /* log2 of blocks per zone */
	int16_t pad2; /* make things line up again */
	uint32_t max_file; /* maximum file size */
	uint32_t zones; /* number of zones on disk */
	int16_t magic; /* magic number */
	int16_t pad3; /* make things line up again */
	uint16_t blocksize; /* block size in bytes */
	uint8_t subversion; /* filesystem sub–version */
} superblock;

#define DIRECT_ZONES 7
typedef struct 
{
	uint16_t mode; /* mode */
	uint16_t links; /* number or links */
	uint16_t uid;
	uint16_t gid;
	uint32_t size;
	int32_t atime;
	int32_t mtime;
	int32_t ctime;
	uint32_t zone[DIRECT_ZONES];
	uint32_t indirect;
	uint32_t two_indirect;
	uint32_t unused;
}inode;

/*File Types*/
FILE *openImage(char *fname);
superblock getSuperBlockData(FILE *img);
inode getInode(FILE *img, superblock sb);

// int getPartitions(int part, int subpart, FILE *img);

#endif
