#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "minhelper.h"

void printCurFile(directory dir, inode i_info);
void printDirContents(uint16_t zoneSize, superblock sb, uint32_t part_offset,
                      FILE *img);
void printPermissions(uint16_t mode);

int main(int argc, char *argv[]){
  int verbose = 0;
  int sub_part = NO_SUBPART;
  int pri_part = NO_PRIPART;
  int opt, part_offset = 0;
  char * image_name;
  char * path = NULL;
  superblock sb; 
  uint16_t zone_size;
  FILE * img;

  while ((opt = getopt(argc, argv, "vp:s:")) != -1){
    switch(opt){
      case 'v': {
        printf("verbose\n");
        verbose = 1;
        break;
      }
      case 'p':{
        printf("partition: %s\n", optarg); 
        pri_part = atoi(optarg);
        break;
      }
      case 's':{
        printf("subpartition: %s\n", optarg);
        sub_part = atoi(optarg);
        break;
      }
      case '?':{
       exit(EXIT_FAILURE);
      }
    }
  }
  
  if (sub_part != NO_SUBPART && pri_part == NO_PRIPART){
    printf("usage: minls [-v] [ -p part [-s subpart] ] imagefile [path]\n");
    exit(EXIT_FAILURE);
  } 

  if (argv[optind] == NULL){
    exit(EXIT_FAILURE);
  }else{
    image_name = argv[optind];
    path = argv[optind + 1]; 
  }

  printf("image: %s\n", image_name);
  printf("path: %s\n", path);
  
  /* open the image */
  img = openImage(image_name);
  
  if (pri_part != NO_PRIPART)
    part_offset = get_partition(img, pri_part, sub_part, verbose); 

  printf("looking for superblock at: %x\n", part_offset + OFFSET);
  sb = getSuperBlockData(img, part_offset + OFFSET, verbose);
  zone_size = getZoneSize(sb);
  printDirContents(zone_size, sb, part_offset, img); 
  
  exit(EXIT_SUCCESS); 
}

void printCurFile(directory dir, inode i_info)
{
  /*drwxr-xr-x 64 .*/
    printf("%9u %s\n", i_info.size, dir.name);
}

void printDirContents(uint16_t zoneSize, superblock sb, uint32_t part_offset, 
                      FILE *img)
{
  directory dir;
  inode i_info;
  uint32_t i = 0;
  while(i >= 0)
  {
    dir = getZone(sb, zoneSize, i, part_offset, img);
    /*if name is null, at end of directory*/
    if(dir.name[0] == '\0')
    {
      break;
    }
    /*if dir or file exists*/
    else if(dir.inode != 0)
    {
      /*add inode info like read permission*/
      i_info = getInode(img, sb, dir.inode, part_offset);
      printPermissions(i_info.mode);
      printCurFile(dir, i_info);
    }
    i++;    
  }
}
void printPermissions(uint16_t mode)
{
  char perm[11] = "";
  char r = 'r';
  char w = 'w';
  char x = 'x';
  char d = 'd';
  char not = '-';
  (mode & IS_DIR) ? strncat(perm, &d, 1) : strncat(perm, &not, 1);
  (mode & OWN_R) ? strncat(perm, &r, 1) : strncat(perm, &not, 1);
  (mode & OWN_W) ? strncat(perm, &w, 1) : strncat(perm, &not, 1);
  (mode & OWN_X) ? strncat(perm, &x, 1) : strncat(perm, &not, 1);
  (mode & GRP_R) ? strncat(perm, &r, 1) : strncat(perm, &not, 1);
  (mode & GRP_W) ? strncat(perm, &w, 1) : strncat(perm, &not, 1);
  (mode & GRP_X) ? strncat(perm, &x, 1) : strncat(perm, &not, 1);
  (mode & OTR_R) ? strncat(perm, &r, 1) : strncat(perm, &not, 1);
  (mode & OTR_W) ? strncat(perm, &w, 1) : strncat(perm, &not, 1);
  (mode & OTR_X) ? strncat(perm, &x, 1) : strncat(perm, &not, 1);
  printf("%s ", perm);
}
