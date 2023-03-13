#include "minhelper.h"
/* This contains helper functions that are used by both minget and minls */

/*Finding filesystem*/
FILE *openImage(char *fname)
{
	FILE *img_file;
	img_file = fopen(fname, "r");
	if(img_file == NULL)
	{
		perror("can't open file");
		exit(EXIT_FAILURE);
	}
	return img_file;
}
/*Finding superblock*/
superblock getSuperBlockData(FILE *img)
{
	int foundBlock;
	superblock sblock;
	foundBlock = fseek(img, OFFSET, SEEK_SET);
	if(foundBlock != 0)
	{
		perror("can't seek");
		exit(foundBlock);
	}
	fread(&sblock, sizeof(superblock), 1, img);
	if(sblock.magic != MINIX_MAGIC)
	{
		printf("Bad magic number. (0x%04x)\nThis doesn't look like a MINIX filesystem\n", sblock.magic);
		exit(EXIT_FAILURE);
	}
	return sblock;
}	
/*Finding inodes*/
inode getInode(FILE *img, superblock sb)
{
	inode i;
	uint16_t zonesize = sb.blocksize << sb.log_zone_size;
	fseek(img, (sb.blocksize*4) , SEEK_SET);
	fread(&i, sizeof(inode), 1, img);
	return i;
}
	
// /*Checking arguments*/

// /*Error checking*/


