#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE_DISK 16777216

#define BYTES_PER_SECTOR 512
#define SECTORS_PER_CLUSTER 4
#define RESERVED_SECTOR 1

typedef struct BootRecord
{
  unsigned short    bytes_per_sector;
  unsigned char     sectors_per_cluster;
  unsigned short    reserved_sector;
  unsigned short    sector_directory_blocks;
  unsigned short    sector_section_data;
  unsigned int      pointer_to_space_free;
  unsigned int      total_directory_entries;
}__attribute__((packed)) BootRecord;

typedef struct FileFormat{    
    unsigned char status;
    unsigned char name[12];
    unsigned char ext;
    unsigned char attribute;
    unsigned int  first_cluster;
    unsigned int  fileSize;
} __attribute__((packed)) FileFormat;

void openFile(char * filename);
void createImg();