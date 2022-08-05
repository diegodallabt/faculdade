#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define BYTES_PER_SECTOR 512
#define SECTORS_PER_CLUSTER 4
#define RESERVED_SECTOR 1

#pragma pack(push, 1)
typedef struct BootRecord
{
  unsigned short    bytes_per_sector;
  unsigned char     sectors_per_cluster;
  unsigned char     reserved_sector;
  unsigned short    sector_directory_blocks;
  unsigned short    sector_section_data;
  unsigned int      pointer_to_space_free;
  unsigned int      total_main_directory_entries;
}__attribute__((packed)) BootRecord;

#pragma pack(push, 1)
typedef struct FileFormat{    
    unsigned char  status;
    unsigned char  name[12];
    unsigned char  ext[4];
    unsigned char  attribute;
    unsigned int   first_cluster;
    unsigned int   fileSize;
    unsigned short total_entries;
} __attribute__((packed)) FileFormat;

bool setBR(int TOTAL_SECTORS);
BootRecord readBR();
int startDirectoryBlocks();
int startDataSection();
int searchCluster(int cluster);
void insertBlocksFree(int qtClusters, int firstCluster);
void deleteFile(int root, int adress, int entries);
void reallocateFirstPointer(int qtClusters);
bool inicializeLinkList();
void createImage();
void createDir(int adress, int total_entries);
void copyFile(char * filename, int root, int adress, int entries);
void extractArq(char * filename, int adress, int entries);
void showFiles(char * filename, int adress, int root, int entries);
int next(int endAtual, int entries);
void navigator(char *filename);