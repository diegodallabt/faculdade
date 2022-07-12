#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIZE_DISK 16777216

#define BYTES_PER_SECTOR 256
#define SECTORS_PER_CLUSTER 2
#define RESERVED_SECTOR 2

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

BootRecord setBR(FILE * img){
    BootRecord br;
    
    br.bytes_per_sector = BYTES_PER_SECTOR;
    br.sectors_per_cluster = SECTORS_PER_CLUSTER;
    br.reserved_sector = RESERVED_SECTOR;
    br.sector_directory_blocks = (SIZE_DISK/BYTES_PER_SECTOR)*0.1;
    br.sector_section_data = (SIZE_DISK/BYTES_PER_SECTOR)*0.9;
    br.pointer_to_space_free = 0;
    br.total_directory_entries = 0;

    fseek(img, 0, SEEK_SET);
    fwrite(&br, 1 , sizeof(br) , img);

    return br;
}


void createImage(){
    FILE * img;
    char nulo[1] = {0};

    img = fopen( "file.img" , "wb" );

    if(img == NULL){
        printf("Error opening image file.\n");
    }

    for(int i=0 ; i<= SIZE_DISK ; i++){
        fwrite(nulo, 1 , sizeof(nulo) , img);
    }
    setBR(img);

    fclose(img);
}


int main(int argc, char const *argv[])
{
    createImage();
    return(0);
}